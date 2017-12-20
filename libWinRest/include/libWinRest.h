// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� LIBWINREST_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// LIBWINREST_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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
