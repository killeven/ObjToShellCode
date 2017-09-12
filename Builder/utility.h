#ifndef COMMON_UTILITY_H
#define COMMON_UTILITY_H
#include "common.h"
#include <string>

using std::string;

class FileLoader {
public:
  FileLoader(std::string& path);
  ~FileLoader();
  inline char* base() { 
    return buffer_base_; 
  };
  inline size_t size() { 
    return total_size_; 
  };
  inline int loaded() { 
    return loaded_; 
  };
private:
  int loadfile(std::string& path);
  void closefile();
private:
  std::string filename_;
  HANDLE file_handle_, map_handle_;
  char* buffer_base_;
  size_t total_size_;
  int loaded_;
};

string extract_filename(string& path);
string decode_fun(string& name);
uint32_t HashStringFromRol3Xor(const char* sz);

#endif  // COMMON_UTILITY_H
