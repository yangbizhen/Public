#include "TCPClient.h"
#include "boost/algorithm/string.hpp"
using namespace libNetUtil;
#define LOG(p) /*cout<<p<<"\n";*/
#define LOG_DEBUG__(p) LOG(p)
#define LOG_INFO__(p) LOG(p)
#define LOG_WARN__(p) LOG(p)

TCPClient::TCPClient(string host/*="127.0.0.1"*/, string port/*="10000"*/,string token)
:m_host(host), m_port(port), m_token(token), m_socket(m_io_service), m_resolver(m_io_service),
owner_async_write_handler_(0), owner_async_read_handler_(0), m_is_connected(false), m_read_end_delim("\r\n"), m_is_inited(false)
{

}


TCPClient::~TCPClient()
{
}

void libNetUtil::TCPClient::start_service()
{
	if (!this->m_is_inited)
	{
		this->connect();
		this->m_is_inited = true;
	}
	else
	{
		this->m_io_service.reset();
	}
	this->m_io_service.run();
}

void libNetUtil::TCPClient::async_resolve_handler(const boost::system::error_code& ec, tcp::resolver::iterator iter)
{
	if (!ec)
	{
		m_socket.close();
		boost::asio::async_connect(m_socket, iter, boost::bind(&TCPClient::async_connect_handler, this, bap_error, bap_iter));
		LOG_DEBUG__("resolve successed:"<<iter->endpoint());
	}
}

void libNetUtil::TCPClient::async_connect_handler(const boost::system::error_code& ec, tcp::resolver::iterator iter)
{
	if (!ec)
	{
		LOG_INFO__("connect successed:" << iter->endpoint());
		m_is_connected = true;
		string strToken = "{\"token\":\"" + m_token + "\"}";
		this->async_write(strToken.c_str(),strToken.length());
		this->async_read();
	}
	else
	{
		this->reconnect();
	}
}

void libNetUtil::TCPClient::async_read_handler(const boost::system::error_code& ec, std::size_t sz, int pData)
{
	boost::asio::streambuf* pbuf = (boost::asio::streambuf*)pData;
	if (ec/* == boost::asio::error::eof*/)
	{
		m_is_connected = false;
		delete pbuf;
		this->reconnect();
		return;
	}
	boost::asio::streambuf::const_buffers_type cbt = pbuf->data();
	string str(boost::asio::buffers_begin(cbt), boost::asio::buffers_end(cbt));
	//std::cout << str << endl;
	boost::erase_last(str, m_read_end_delim);//  [8/30/2016 gxy]ÒÆ³ý·Ö¸ô·û
	LOG_DEBUG__("receive: "<<str);
#if 0
	if (this->owner_async_read_handler_)
	{
		owner_async_read_handler_(ec, str.size(), (char*)str.c_str());
	}
#else
	if (m_param.rwhandler)
	{
		m_param.rwhandler->asyncRHandler(ec.value(), (void*)str.data(), str.size());
	}
#endif
	delete pbuf;
	this->async_read();
}

void libNetUtil::TCPClient::reconnect()
{
	static size_t cnt = 0;
	cnt %= 10;
	if (cnt++ == 0)
	{
		LOG_WARN__(m_host<<":"<<m_port<<" connect failed,will reconnect in 1(s)");
	}
	boost::this_thread::sleep(boost::posix_time::seconds(1));
	this->connect();
}

void libNetUtil::TCPClient::connect()
{
	tcp::resolver::query query(m_host, m_port);
	m_resolver.async_resolve(query, boost::bind(&TCPClient::async_resolve_handler, this,bap_error,bap_iter));
}

string libNetUtil::TCPClient::make_daytime_string()
{
	time_t now = time(0);
	return ctime(&now);
}

void libNetUtil::TCPClient::async_write_handler(const boost::system::error_code& ec, std::size_t sz, int pData, int param)
{
	if (pData)
	{
		delete[] (char*)pData;
	}
#if 0
	if (this->owner_async_write_handler_)
	{
		owner_async_write_handler_(ec, sz, (void*)param);
	}
#else
	if (m_param.rwhandler)
	{
		m_param.rwhandler->asyncWHandler(ec.value(), (void*)pData, sz, (void*)param);
	}
#endif
}

void libNetUtil::TCPClient::async_write(const char* data, size_t sz, void *param /*= NULL*/)
{
	if (sz > 0)
	{
		char* pData = new char[sz];
		memcpy(pData, data, sz);
		boost::asio::async_write(m_socket, boost::asio::buffer(pData,sz), boost::bind(&TCPClient::async_write_handler, this, bap_error, bap_iter,(int)pData,(int)param));
	}
}

void libNetUtil::TCPClient::set_owner_async_write_handler(boost::function<void(const boost::system::error_code& ec, size_t sz, void* param)> handler)
{
	owner_async_write_handler_= handler;
}

void libNetUtil::TCPClient::set_owner_async_read_handler(boost::function<void(const boost::system::error_code& ec, size_t sz, char* data)> handler)
{
	owner_async_read_handler_ = handler;
}

bool libNetUtil::TCPClient::is_connected()
{
	return m_is_connected;
}

void libNetUtil::TCPClient::async_start_service()
{
	boost::thread th(boost::bind(&TCPClient::start_service,this));
}

void libNetUtil::TCPClient::async_read()
{
	//char* pData = new char[read_buff_sz_];
	//memset(pData, 0, sizeof(char)*read_buff_sz_);
	//boost::asio::async_read(m_socket, boost::asio::buffer(pData, read_buff_sz_), boost::bind(&TCPClient::async_read_handler, this, bap_error, bap_iter, (int)pData));
	boost::asio::streambuf* buff = new boost::asio::streambuf();
	boost::asio::async_read_until(m_socket, *buff,m_read_end_delim, boost::bind(&TCPClient::async_read_handler, this, bap_error, bap_iter, (int)buff));
}


void libNetUtil::TCPClient::stop_service()
{
	if (m_is_connected)
	{
		m_socket.close();
		//m_io_service.stop();
		m_is_connected = false;
	}
	m_io_service.stop();
}

void libNetUtil::TCPClient::set_read_end_delim(string delim)
{
	m_read_end_delim = delim;
}

bool libNetUtil::TCPClient::init(const Param& param)
{
	m_param = param;
	{
		m_host = m_param.host;
		m_port = std::to_string(m_param.port);
		m_read_end_delim = m_param.rEndDelim;
	}
	async_start_service();
	return true;
}

bool libNetUtil::TCPClient::asyncWrite(void* data, size_t sz, void* user)
{
	if (!is_connected())
	{
		return false;
	}
	async_write((const char*)data, sz, user);
	return true;
}

bool libNetUtil::TCPClient::release()
{
	stop_service();
	return true;
}

bool libNetUtil::TCPClient::isConnected()
{
	return is_connected();
}
