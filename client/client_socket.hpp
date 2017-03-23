#pragma once

#include <string>
#include <netinet/in.h>
#include <unistd.h>

class client_error : std::exception
{
public:
	client_error(const char * msg) throw(): msg_(msg){}
	const char * what() const throw() 
	{
		return msg_.c_str();
	} 
private:
	std::string msg_;
};

class Client_socket
{
public:
	Client_socket(){};
	virtual ~Client_socket(){};
	virtual void sock_send(std::string msg) = 0;
	virtual void sock_recieve(char * msg, size_t size) = 0;
	virtual void sock_close() = 0;

protected:
	int sock_;
	struct sockaddr_in sock_info_;
	struct sockaddr * addr_;
	socklen_t len_;

};


class TCP_client_socket : public Client_socket
{
public:
	~TCP_client_socket(){}

	TCP_client_socket(int port);

	void sock_send (std::string msg);

	inline void sock_recieve(char * msg, size_t size)
	{
		if (recv(sock_, msg, size, 0) == -1)
			throw client_error("Recieve error");
	}

	inline void sock_close()
	{
		close(sock_);
	}

};

class UDP_client_socket : public Client_socket
{
public:
	~UDP_client_socket(){}

	UDP_client_socket (int port);

	void sock_send(std::string msg);

	inline void sock_recieve(char * msg, size_t size)
	{
		if (recvfrom(sock_, msg, size, 0, addr_, &len_) == -1)
			throw client_error("Recieve error");
	}

	inline void sock_close()
	{
		close(sock_);
	}

};
