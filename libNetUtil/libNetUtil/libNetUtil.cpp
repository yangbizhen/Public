// libWinRest.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "../include/libNetUtil.h"
#include "string"
using namespace std;
//

#define LOG_E(a) cout<<a<<"\n";
#include "TCPClient.h"
using namespace libNetUtil;
std::shared_ptr<ITCPClient> libNetUtil::ITCPClient::makeSharedInst()
{
	return std::make_shared<TCPClient>();
}

class Callback :public libNetUtil::ITCPClient::IAsyncRWHandler{
	void asyncRHandler(int errCode, void* data, size_t sz){
		int i = 0;
		char *s = (char*)data;
	}
	void asyncWHandler(int errCode, void* data, size_t sz, void* user) {
		int i = 0;
		char *s = (char*)data;
	}
};
void main()
{
	Callback calback;
	std::shared_ptr<ITCPClient> c = ITCPClient::makeSharedInst();
	ITCPClient::Param p;
	p.host = "192.168.1.39";
	p.port = 9005;
	p.rEndDelim = "\n";
	p.rwhandler = &calback;
	c->init(p);
	string s = "hello world";
	c->asyncWrite((void*)s.data(),s.size(),nullptr);
	getchar();
	c->release();
}