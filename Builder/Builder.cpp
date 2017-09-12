// Builder.cpp : 定义控制台应用程序的入口点。
//

#include "common.h"
#include "engine.h"

void* __stdcall CreateInstance() {
  return new ShellCodeEngine::Builder();
}

void __stdcall DeleteInstance(ShellCodeEngine::Builder* builder) {
  delete builder;
}

int __stdcall AddObj(ShellCodeEngine::Builder* builder, char* objpath) {
  std::string path(objpath);
  return builder->add_object_file(path);
}

int __stdcall AddModule(ShellCodeEngine::Builder* builder, char* modulepath) {
  std::string path(modulepath);
  return builder->add_module(path);
}

int __stdcall Build(ShellCodeEngine::Builder* builder, char* savepath, char* entryname) {
  std::string path(savepath);
  std::string entry(entryname);
  return builder->build(path, entry);
}

typedef void(__stdcall *xCallBack)(const char* name);

void __stdcall GetFuncList(ShellCodeEngine::Builder* builder, xCallBack callback) {
  std::vector<std::string> func_list;
  builder->get_function_list(func_list);
  for (auto i : func_list) {
    if (callback != nullptr) callback(i.c_str());
  }
}


int main()
{
  auto builder = new ShellCodeEngine::Builder;
  std::string temp("user32.dll");
  builder->add_module(temp);
  temp.clear();
  temp = "D:\\桌面\\CSharp_Test\\ConsoleApplication1\\Release\\ConsoleApplication1.obj";
  builder->add_object_file(temp);
  std::vector<std::string> func_list;
  builder->get_function_list(func_list);
  for (auto i : func_list) {
    printf("%s\r\n", i.c_str());
  }
  std::string entry = "?entry@@YAXXZ";
  std::string path = "C:\\test.bin";
  builder->build(path, entry);
  system("PAUSE");
}


BOOL APIENTRY DllMain(HMODULE hModule,
  DWORD  ul_reason_for_call,
  LPVOID lpReserved
  )
{
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}
