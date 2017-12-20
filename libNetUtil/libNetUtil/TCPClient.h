#pragma once
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/function.hpp>
#include <boost/array.hpp>
#include <vector>
using namespace std;
using boost::asio::ip::tcp;
#define bap_error boost::asio::placeholders::error
#define bap_iter boost::asio::placeholders::iterator
#include "../include/libNetUtil.h"
namespace libNetUtil
{
	class TCPClient :public ITCPClient
	{
	public:
		static string make_daytime_string();

	public:
		TCPClient(string host="127.0.0.1",string port="10000",string token="");
		~TCPClient();
	public:
		void start_service();
		void async_start_service();
		void stop_service();
		void async_write(const char* data, size_t sz, void *param = NULL);
		void async_read();
		void set_owner_async_write_handler(boost::function<void(const boost::system::error_code& ec, size_t sz, void* param)> handler);
		void set_owner_async_read_handler(boost::function<void(const boost::system::error_code& ec, size_t sz, char* data)> handler);
		bool is_connected();
		void set_read_buf_sz(size_t sz);
		void set_read_end_delim(string delim);
	private:
		void async_resolve_handler(const boost::system::error_code& ec, tcp::resolver::iterator iter);
		void async_connect_handler(const boost::system::error_code& ec, tcp::resolver::iterator iter);
		void async_read_handler(const boost::system::error_code& ec,std::size_t sz, int pData);
		void async_write_handler(const boost::system::error_code& ec, std::size_t sz, int pData,int param);
		void resolve();
		void reconnect();
		void connect();

		virtual bool init(const Param& param) override;
		virtual bool asyncWrite(void* data, size_t sz,void* user) override;
		virtual bool release() override;

		virtual bool isConnected() override;

	protected:
		boost::asio::io_service m_io_service;
		tcp::socket m_socket;
		string m_host;
		string m_port;
		string m_token;
		boost::function<void(const boost::system::error_code& ec, size_t sz, void* param)> owner_async_write_handler_;
		boost::function<void(const boost::system::error_code& ec, size_t sz, char* data)> owner_async_read_handler_;
		volatile bool m_is_connected;
		string m_read_end_delim;//读取结束标志
		tcp::resolver m_resolver;
		volatile bool m_is_inited;
		ITCPClient::Param m_param;
	};
}


