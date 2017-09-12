#ifndef COMMON_COMMON_H
#define COMMON_COMMON_H
#include <stdio.h>
#include <stdint.h>
#include <tchar.h>
#include <windows.h>

#define SAFE_CLOSE(x) do { \
  if(x != nullptr) \
    CloseHandle(x); \
    x = nullptr; \
  } while(0);

#define SAFE_RELEASE(x) do { \
  if(x != nullptr) \
    delete x; \
    x = nullptr; \
  } while(0);

#define ASSERT(x) do { \
  if (!(x)) \
    assert(#x, __FILE__, __LINE__); \
  } while (0);

#define DBGOUT(fmt, ...) do { \
  debug(fmt, __VA_ARGS__); \
  } while (0);

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus
  void __cdecl debug(char* fmt, ...);
  void assert(char* code, char* file, int line);
#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // COMMON_COMMON_H