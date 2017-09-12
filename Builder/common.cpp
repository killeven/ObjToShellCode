#include "common.h"
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

void _cdecl debug(char* fmt, ...) {
  va_list va;
  va_start(va, fmt);
  char o[1024];
  wvnsprintfA(o, sizeof(o) / sizeof(o[0]), fmt, va);
  OutputDebugStringA(o);
  va_end(va);
}

void assert(char* code, char* file, int line) {
  char msg[4096];
  char msgerr[1024];
  uint32_t errcode = GetLastError();

  if (!FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errcode, 0, msgerr, sizeof(msgerr), NULL))
    msgerr[0] = 0;

  int cb = 0;

  cb += wsprintfA(msg + cb, "应用程序错误自检...\r\n\r\n");
  cb += wsprintfA(msg + cb, "===================================================================================\r\n\r\n");
  cb += wsprintfA(msg + cb, " 代码 : %s\r\n", code);
  cb += wsprintfA(msg + cb, " 文件 : %s\r\n", file);
  cb += wsprintfA(msg + cb, " 行号 : %d\r\n", line);
  cb += wsprintfA(msg + cb, " 错误 : [%d]%s\r\n", errcode, msgerr);
  cb += wsprintfA(msg + cb, "===================================================================================\r\n\r\n");
  cb += wsprintfA(msg + cb, "\r\n");
  MessageBoxA(NULL, msg, "DBG_ASSERT", MB_SERVICE_NOTIFICATION + MB_OK);
  SetLastError(errcode);
  abort();
}


