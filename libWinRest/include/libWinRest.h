// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 LIBWINREST_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// LIBWINREST_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#pragma once
#ifdef LIBWINREST_EXPORTS
#define LIBWINREST_API __declspec(dllexport)
#else
#define LIBWINREST_API __declspec(dllimport)
#endif
#include <string>
using namespace std;

namespace libWinRest
{
	LIBWINREST_API bool httpPostJson(const string& url, const string& json, string* respBody = nullptr,size_t timeOutMs = 1000);
	LIBWINREST_API bool httpGet(const string& url,string* respBody = nullptr, size_t timeOutMs = 1000);
}
