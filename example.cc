//
// Created by leviathan on 2017/12/20.
//

#include <string>

#include "db/bitcask.h"

int main() {
  biu::Bitcask db;
  biu::Options options;
  biu::Status s = db.Open(options, "mydb");
  if (!s.ok()) {
    std::cout << s.ToString() << std::endl;
  }

  std::string value (128, 'x');
  s = db.Put("key", value);
  if (!s.ok()) {
    std::cout << s.ToString() << std::endl;
  }
  std::string get_value;
  s = db.Get("key", &get_value);
  if (!s.ok()) {
    std::cout << s.ToString() << std::endl;
  } else {
    std::cout << "key: " << get_value << std::endl;
  }

  s = db.Delete("key");
  if (!s.ok()) {
    std::cout << s.ToString() << std::endl;
  }
  std::string del_value;
  s = db.Get("key", &del_value);
  if (!s.ok()) {
    std::cout << s.ToString() << std::endl;
  } else {
    std::cout <<"key: " << del_value << std::endl;
  }
  return 0;
}
