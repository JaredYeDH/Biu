//
//  bitcask.h
//  Biu
//
//  Created by Leviathan on 17/12/18.
//  Copyright © 2017 Leviathan. All rights reserved.
//

#ifndef SRC_BITCASK_H_
#define SRC_BITCASK_H_

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

namespace biu {

const std::string IndexDirectory = "/index";
const std::string DataDirectory = "/db";
const std::string DataFileName = "data";
const std::string HintFileName = "hint";

// Key-value
struct BitcaskData {
  time_t timestamp;
  int64_t key_len;
  std::string key;
  int64_t value_len;
  std::string value;
};

// Index
struct BitcaskIndex {
  time_t timestamp;
  int64_t key_len;
  std::string key;
  int32_t file_id;
  int64_t data_pos;
  bool vaild;
};

struct Options {
  // The size of data file
  size_t max_file_size;
 
  // The size of index file 
  size_t max_index_size;

  // If this process is going to be a writer and not just a reader
  bool read_write;

  // If this writer would prefer to sync the write file after every write 
  // operation
  bool sync_on_put;

  //Options();
};

class Status;
class Env;
class FileLock;

// Bitcask implement
class Bitcask {
 public:
  Bitcask();

  ~Bitcask();
  
  Status Open(const Options& options, const std::string& dbname);

  Status Put(const std::string& key, const std::string& value);

  Status Get(const std::string& key, std::string* value);

  Status Delete(const std::string& key);

  Status Close();

  FileLock* db_lock_;
  std::string dbname_;
private:
  Options options_;
  std::unordered_map<std::string, BitcaskIndex> index_;
  
  // Active data file
  int32_t active_id_;
  int64_t active_file_size_;
  std::ofstream active_file_;

  // Hint file
  int32_t hint_id_;
  int64_t hint_file_size_;
  std::ofstream hint_file_;

  Env * env_;

  Status Init();
  Status NewFileStream(std::ofstream& file_stream,
                       int32_t & id,
                       int64_t& file_size,
                       const std::string& directory,
                       const std::string& filename);

  int64_t SyncData(const BitcaskData& data);

  Status SyncIndex(const BitcaskIndex& index);

  Status Retrieve(const std::string& key,
                  const int32_t file_id,
                  const int64_t data_pos,
                  time_t* timestamp,
                  std::string* value);

  //Status Merge();

  Status LoadIndex(const std::string& file);

  Bitcask(const Bitcask& b);
  void operator=(const Bitcask& b);
};

class Status {
 public:
  Status() : code_(kOk) { }
  virtual ~Status() { }
  
  static Status OK() { return Status(); }

  static Status NotFound(const std::string& msg) {
    return Status(kNotFound, msg); 
  }
  static Status IOError(const std::string& msg) {
    return Status(kIOError, msg); 
  }

  bool ok() const { return (code_ == kOk); }

  bool IsNotFound() const { return code() == kNotFound; }

  bool IsIOError() const { return code() == kIOError; }

  std::string ToString() const {
      return msg_;
  }
 private:
  enum Code {
    kOk = 0,
    kNotFound = 1,
    kIOError = 2
  };

  Code code_;
  std::string msg_;

  Code code() const {
    return code_;
  }

  Status(Code code, const std::string& msg)
          : code_(code),
          msg_(msg) { }
};

class FileLock {
 public:
  FileLock() { }
  ~FileLock() { }

    int fd_;
    std::string name_;
 private:
  // No copying allowd
  FileLock(const FileLock&);
  void operator=(const FileLock&);
};

class Env {
 public:
  Status CreateDir(const std::string& name);
  time_t TimeStamp() {
    time_t timer;
    time(&timer);
    return timer; 
  }

  Status LockFile(const std::string& name, FileLock** l);

  Status UnlockFile(FileLock* l);

  Status GetChildren(const std::string& name, std::vector<std::string>& files);

  int32_t FindMaximumId(const std::vector<std::string>& files);

  bool FileExists(const std::string& name);
 private:
  static int LockOrUnlock(int fd, bool lock);

  Env(const Env& env);
  void operator=(const Env& env);
};

} // namespace biu

#endif // SRC_BITCASK_H_
