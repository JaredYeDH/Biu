//
//  bitcask.cc
//  Biu
//
//  Created by Leviathan on 17/12/18.
//  Copyright © 2017 Leviathan. All rights reserved.
//

#include "bitcask.h"

namespace biu {

Bitcask::Bitcask() 
  : active_id_(0),
    hint_id_(0),
    db_lock_(nullptr),
    env_(nullptr) { }

Bitcask::~Bitcask() {
  this->Close();
  delete env_;
}

Status Bitcask::Open(const Options& options, const std::string& dbname) {
  options_ = options;
  dbname_ = dbname;
  return this->Init();
}

Status Bitcask::Close() {
  Status s;
  if (active_file_.is_open()) {
    active_file_.close();
  }
  if (hint_file_.is_open()) {
    hint_file_.close();
  }
  if (options_.read_write) {
    s = env_->UnlockFile(db_lock_);
    if (!s.ok()) {
      return s;
    }
  }
  return s;
}

Status Bitcask::Init() {
  Status s;
  if (!env_->FileExists(dbname_)) {
    env_->CreateDir(dbname_);
  }

  if (options_.read_write) {
    s = env_->LockFile(dbname_ + LOCK, &db_lock_);
    if (!s.ok()) {
      return s; 
    }
  }

  std::vector<std::string> index_files;
  if (env_->FileExists(dbname_ + IndexDirectory)) {
    s = env_->GetChildren(dbname_ + IndexDirectory, index_files);
    if (!s.ok()) {
      return s;
    }
  } else {
    env_->CreateDir(dbname_ + IndexDirectory);
  }
  
  // Find the maximum id of hint file
  hint_id_ = env_->FindMaximumId(index_files);

  // If have index file, load it to memory
  for (auto file : index_files) {
    this->LoadIndex(file);
  }

  // Find the maximum id of active file
  if (env_->FileExists(dbname_ + DataDirectory)) {
    std::vector<std::string> db_files;
    s = env_->GetChildren(dbname_ + DataDirectory, db_files);
    if (!s.ok()) {
      return s;
    } else {
      active_id_ = env_->FindMaximumId(db_files);
    }
  } else {
    env_->CreateDir(dbname_ + DataDirectory);
  }

  s = this->NewFileStream(active_file_, active_id_, active_file_size_, DataDirectory, DataFileName);
  if (!s.ok()) {
    return s; 
  }
  s = this->NewFileStream(hint_file_, hint_id_, hint_file_size_, IndexDirectory, HintFileName);
  if (!s.ok()) {
    return s; 
  }
  return s;
}

Status Bitcask::NewFileStream(std::ofstream& file_stream,
                              int32_t& id,
                              int64_t& file_size,
                              const std::string& directory,
                              const std::string& filename) {
  Status s;
  if (options_.read_write) { // Writable and readable
    file_stream.open(dbname_ + directory + "/" + filename + std::to_string(id),
                      std::ios::in |
                      std::ios::out|
                      std::ios::app|
                      std::ios::ate|
                      std::ios::binary);
    file_size = file_stream.tellp();
  } else {  // Only readable
    file_stream.open(dbname_ + directory + "/" + filename + std::to_string(id),
                      std::ios::out|
                      std::ios::binary);
  }
  return s;
}

Status Bitcask::Put(const std::string& key, const std::string& value) {
  Status s;
  BitcaskData data;
  data.timestamp = env_->TimeStamp();
  data.key_len = static_cast<int64_t >(key.size());
  data.key = key;
  data.value_len = static_cast<int64_t >(value.size());
  data.value = value;

  BitcaskIndex index;
  index.timestamp = data.timestamp;
  index.key_len = static_cast<int64_t >(key.size());
  index.key = key;

  // write to file
  int64_t pos = this->SyncData(data);
  
  if (pos < 0) {
    return s.IOError("Write data failed."); 
  }

  index.file_id = active_id_;
  index.data_pos = pos;
  index.vaild = true;

  // Update index
  index_[key] = index; 

  s = this->SyncIndex(index);
  if (!s.ok()) {
    return s;
  }
  return s;
}

int64_t Bitcask::SyncData(const BitcaskData& data) {
  int64_t pos = active_file_size_;
  if (active_file_size_ < options_.max_file_size) {
    active_file_.write((char *)(&data.timestamp), sizeof(time_t));
    active_file_.write((char *)(&data.key_len), sizeof(int64_t));
    active_file_.write(data.key.c_str(), data.key_len);
    active_file_.write((char *)(&data.value_len), sizeof(int64_t));
    active_file_.write(data.value.c_str(), data.value_len);
    
    active_file_size_ += sizeof(time_t) + sizeof(int64_t) * 2 + data.key_len + data.value_len;
    active_file_.flush();
    return pos;
  } else {
    active_file_.close();

    // Open new data file
    active_id_++;
    Status s;
    s = this->NewFileStream(active_file_, active_id_, active_file_size_, DataDirectory, DataFileName);
    if (!s.ok()) {
      return -1;
    }
    return this->SyncData(data);
  }
}

Status Bitcask::SyncIndex(const BitcaskIndex& index) {
  Status s;
  if (hint_file_size_ < options_.max_index_size) {
    hint_file_.write((char* )(&index.timestamp), sizeof(time_t));
    hint_file_.write((char* )(&index.key_len), sizeof(int64_t));
    hint_file_.write(index.key.c_str(), index.key_len);
    hint_file_.write((char* )(&index.file_id), sizeof(int32_t));
    hint_file_.write((char* )(&index.data_pos), sizeof(int64_t));
    
    hint_file_size_ += sizeof(time_t) + sizeof(int32_t) + sizeof(int64_t) * 2 + index.key_len;
    hint_file_.flush();
    return s;
  } else {
    hint_file_.close();

    // Open new index file
    hint_id_++;
    s = this->NewFileStream(hint_file_, hint_id_, hint_file_size_, IndexDirectory, HintFileName);
    if (!s.ok()) {
      return s; 
    }
    return this->SyncIndex(index);
  }
}

Status Bitcask::Get(const std::string& key, std::string* value) {
  Status s;
  auto find = index_.find(key);
  if (find != index_.end() && find->second.vaild) {
    int32_t file_id = find->second.file_id;
    int64_t data_pos = find->second.data_pos; 
    time_t timestamp;
    s = this->Retrieve(key, file_id, data_pos, &timestamp, value);
    return s;
  } else {
    return s.NotFound("Not Found"); 
  }
}

Status Bitcask::Delete(const std::string& key) {
  Status s;
  auto find = index_.find(key);
  if (find != index_.end()) {
    find->second.timestamp = env_->TimeStamp();
    find->second.vaild = false;
    index_[find->first] = find->second;
    s = this->SyncIndex(find->second);
    return s;
  }
  return s;
}

Status Bitcask::Retrieve(const std::string& key,
                         const int32_t file_id,
                         const int64_t data_pos,
                         time_t* timestamp,
                         std::string* value) {
  Status s;
  std::ifstream file_stream;
  file_stream.open(dbname_ + DataDirectory + "/" + DataFileName + std::to_string(file_id),
                    std::ios::out|
                    std::ios::binary);
  file_stream.seekg(data_pos, std::ios_base::beg);

  int64_t key_len = 0, value_len = 0;
  file_stream.read((char* )(&timestamp), sizeof(time_t));
  file_stream.read((char* )(&key_len), sizeof(int64_t));

  auto read_key = new char[key_len];
  file_stream.read(read_key, key_len);

  file_stream.read((char* )(&value_len), sizeof(int64_t));

  auto read_value = new char[value_len];
  file_stream.read(read_value, value_len);
  value->append(read_value, static_cast<unsigned long>(value_len));

  if (read_key == key) {
    return s; 
  } else {
    return s.IOError("Data position incorrect.");
  }
}

Status Bitcask::LoadIndex(const std::string &file) {
  Status s;
  std::ifstream file_stream;
  file_stream.open(dbname_ + DataDirectory + "/" + file,
                    std::ios::out|
                    std::ios::binary);
  file_stream.seekg(0, std::ios_base::beg);

  while (file_stream.eof()) {
    BitcaskIndex index;
    file_stream.read((char* )(&index.timestamp), sizeof(time_t));
    file_stream.read((char* )(&index.key_len), sizeof(int64_t));
    auto read_key = new char[index.key_len];
    file_stream.read(read_key, index.key_len);
    file_stream.read((char* )(&index.file_id), sizeof(int32_t));
    file_stream.read((char* )(&index.data_pos), sizeof(int64_t));
    file_stream.read((char* )(&index.vaild), sizeof(bool));

    if (index.vaild) {
      index_[index.key] = index;
    }
  }
  
  return s;
}

Status Env::CreateDir(const std::string& name) {
  Status s;
  if (mkdir(name.c_str(), 0755) != 0) {
    return s.IOError("Create directory failed."); 
  }
  return s;
}

Status Env::LockFile(const std::string& name, FileLock** lock) {
  *lock = nullptr;
  Status s;
  int fd = ::open(name.c_str(), O_RDWR | O_CREAT, 0644);
  if (fd < 0) {
    return s.IOError("Lock file failed." + std::to_string(errno));
  } else if (LockOrUnlock(fd, true) == -1) {
    ::close(fd);
    return s.IOError("Lock file failed." + std::to_string(errno));
  } else {
    FileLock* my_lock = new FileLock;
    my_lock->fd_ = fd;
    my_lock->name_ = name;
    *lock = my_lock; 
  }
  return s;
}

Status Env::UnlockFile(FileLock* lock) {
  Status s;
  if (LockOrUnlock(lock->fd_, false) == -1) {
    return s.IOError("Unlock file failed."); 
  }
  ::close(lock->fd_);
  delete lock;
  return s;
}

int Env::LockOrUnlock(int fd, bool lock) {
  errno = 0;
  struct flock f;
  memset(&f, 0, sizeof(f));
  f.l_type = (lock ? F_WRLCK : F_UNLCK);
  f.l_whence = SEEK_SET;
  f.l_start = 0;
  f.l_len = 0;
  return fcntl(fd, F_SETLK, &f);
}

Status Env::GetChildren(const std::string& name, std::vector<std::string>& files) {
  Status s;
  files.clear();
  DIR* d = opendir(name.c_str());
  if (d == nullptr) {
    return s.IOError("Get directory children failed.");
  }
  struct dirent* entry;
  while ((entry = readdir(d)) != nullptr) {
    if (strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0) {
      continue;
    }
    files.push_back(entry->d_name);
  }
  closedir(d);
  return s;
}

int32_t Env::FindMaximumId(const std::vector<std::string> &files) {
  int32_t max = 0;
  char c_max;
  for (auto file : files) {
    c_max = static_cast<char >(file[file.size()] - 1);
    if (max < static_cast<int >(c_max)) {
      max = static_cast<int >(c_max);
    }
  }
  return max;
}

bool Env::FileExists(const std::string& name) {
  return access(name.c_str(), F_OK) == 0;
}

} // namespace biu

