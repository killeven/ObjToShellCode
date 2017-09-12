#include "utility.h"

string extract_filename(string& path) {
  string ret = "";
  auto pos = path.find_last_of("\\");
  if (pos != string::npos) {
    ret = path.substr(pos + 1, path.length());
  }
  else {
    ret = path;
  }
  return ret;
}

string decode_fun(string& name) {
  auto ret = name;
  if (name.length() < 2) return ret;
  if (name[0] == '?') {
    ret = name;
  } else {
    auto pos = name.find("__imp_");
    if (pos != string::npos) ret = name.substr(pos + 6, name.length() - pos - 6);
    if (ret[0] == '_') ret = ret.substr(1, name.length() - 1);
    pos = ret.find("@");
    if (pos != string::npos) ret = ret.substr(0, pos);
  }
  return ret;
}

uint32_t HashStringFromRol3Xor(const char* sz) {
  _asm {
    mov eax, sz
      push esi
      cdq
      test eax, eax
      jz finished
      mov esi, eax
      xor eax, eax
      calc_hash :
    lodsb
      test al, al
      jz finished
      rol edx, 3
      xor dl, al
      jmp calc_hash
      finished :
    mov eax, edx
      pop esi
  }
}

FileLoader::FileLoader(std::string& path): buffer_base_(nullptr), filename_(path) {
  loaded_ = loadfile(path);
}

FileLoader::~FileLoader() {
  closefile();
}

int FileLoader::loadfile(std::string& path) {
  file_handle_ = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (file_handle_ == INVALID_HANDLE_VALUE) return 0;
  total_size_ = GetFileSize(file_handle_, nullptr);
  map_handle_ = CreateFileMapping(file_handle_, nullptr, PAGE_READONLY, 0, 0, nullptr);
  if (map_handle_ == nullptr) {
    SAFE_CLOSE(file_handle_);
    return 0;
  }
  buffer_base_ = (char*)MapViewOfFile(map_handle_, FILE_MAP_READ, 0, 0, 0);
  if (buffer_base_ == nullptr) {
    SAFE_CLOSE(map_handle_);
    SAFE_CLOSE(file_handle_);
    return 0;
  }
  return 1;
}

void FileLoader::closefile() {
  if (buffer_base_ != nullptr)
    UnmapViewOfFile((void*)buffer_base_);
  SAFE_CLOSE(map_handle_);
  SAFE_CLOSE(file_handle_);
}