// libWinRest.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "../include/libWinRest.h"
//
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
//
#define LOG_E(a) cout<<a<<"\n";

LIBWINREST_API bool libWinRest::httpPostJson(const string& url, const string& json, string* respBody /*= nullptr*/, size_t timeOutMs/*=1000*/)
{
	bool ret = false;
	utility::string_t url_t = utility::conversions::to_string_t(url);
	utility::string_t json_t = utility::conversions::to_string_t(json);
	utility::string_t query_t = uri_builder().to_string();
	http_client_config cfg;
	cfg.set_buffer_request(true);
	cfg.set_timeout(std::chrono::milliseconds(timeOutMs));
	http_client client(url_t, cfg);
	try
	{
		client.request(methods::POST, query_t, json_t).then([&](http_response resp)
		{
			if (resp.status_code() == status_codes::OK)
			{
				if (respBody)
				{
					auto v = resp.extract_vector().get();
					respBody->assign(v.begin(), v.end());
				}
				ret = true;
			}
		}).wait();
	}
	catch (std::exception& e)
	{
		LOG_E(e.what());
	}
	return ret;
}

LIBWINREST_API bool libWinRest::httpGet(const string& url, string* respBody /*= nullptr*/, size_t timeOutMs /*= 1000*/)
{
	bool ret = false;
	utility::string_t url_t = utility::conversions::to_string_t(url);
	http_client_config cfg;
	cfg.set_buffer_request(true);
	cfg.set_timeout(std::chrono::milliseconds(timeOutMs));
	http_client client(url_t, cfg);
	try
	{
		client.request(methods::GET).then([&](http_response resp)
		{
			if (resp.status_code() == status_codes::OK)
			{
				if (respBody)
				{
					auto v = resp.extract_vector().get();
					respBody->assign(v.begin(), v.end());
				}
				ret = true;
			}
		}).wait();
	}
	catch (std::exception& e)
	{
		LOG_E(e.what());
	}
	return ret;
}


void main()
{
	string url = "http://192.168.1.79:9000/deviceRegist";
	string json = "{\"deviceName\":\"gxy-test\",\"deviceType\":\"YZHD-VSTS-C2\",\"modelNumber\":\"ga-A5320\",\"physicalId\":\"gxy-123456789123\"}";
	string resp;
	libWinRest::httpPostJson(url, json, &resp);
	libWinRest::httpGet("http://192.168.1.79:9000/getInformation?type=unixTime", &resp);
	getchar();
}