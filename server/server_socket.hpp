#pragma once

#include <unistd.h>
#include <string>

class server_error : std::exception
{
public:
	server_error(const char * msg) throw(): msg_(msg){}
	const char * what() const throw() 
	{
		return msg_.c_str();
	} 
private:
	std::string msg_;
};

class Server_socket
{
public:
	Server_socket(){}	
	virtual ~Server_socket(){}	
	virtual void sock_send(std::string msg) = 0;
	virtual char * sock_recieve() = 0;
	virtual void sock_close() = 0;

protected:
	int sock_;
	struct sockaddr_in sock_info_;
	struct sockaddr * addr_;
	socklen_t len_;

};


class TCP_server_socket : public Server_socket
{
public:

	~TCP_server_socket() {}

	TCP_server_socket(int port);

	inline void sock_send(std::string msg)
	{
		if (send(client_, &msg[0], msg.size() + 1, 0) == -1)
			throw server_error("Sending error");	
	}

	char * sock_recieve();

	inline void sock_close()
	{
		close(client_);
		close(sock_);
	}

private:
	int client_;

};

class UDP_server_socket : public Server_socket
{
public:
	~UDP_server_socket() {}
	
	UDP_server_socket(int port);

	inline void sock_send (std::string msg)
	{
		if (sendto(sock_, &msg[0], msg.size() + 1, 0, addr_, len_) == -1)
			throw server_error("Sending error");
	}

	char * sock_recieve ();

	void sock_close()
	{
		close(sock_);
	}

};

