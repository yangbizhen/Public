// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 LIBNETUTIL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// LIBNETUTIL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#pragma once
#ifdef LIBNETUTIL_EXPORTS
#define LIBNETUTIL_API __declspec(dllexport)
#else
#define LIBNETUTIL_API __declspec(dllimport)
#endif
#include <string>
#include <memory>
using namespace std;


namespace libNetUtil
{
	struct LIBNETUTIL_API ITCPClient
	{
		struct IAsyncRWHandler
		{
			virtual void asyncRHandler(int errCode, void* data, size_t sz)=0;
			virtual void asyncWHandler(int errCode, void* data, size_t sz, void* user)=0;
		};
		struct Param
		{
			string host;
			size_t port;
			string rEndDelim;
			IAsyncRWHandler *rwhandler = nullptr;
		};
		static std::shared_ptr<ITCPClient> makeSharedInst();
		virtual bool init(const Param& param)=0;
		virtual bool isConnected() = 0;
		virtual bool asyncWrite(void* data,size_t sz,void* user)=0;
		virtual bool release()=0;
	};
}