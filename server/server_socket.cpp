#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "server_socket.hpp"
using namespace std;


char * TCP_server_socket::sock_recieve ()
{
	int msg_size, net_size;
	if (recv(client_, &net_size, sizeof(net_size), 0) == -1)
		throw server_error("recieve error");
	msg_size = ntohl(net_size);
	char * msg = new char[msg_size];
	if (recv(client_, msg, msg_size, 0) == -1)
		throw server_error("recieve error");		
	return msg;
}

TCP_server_socket::TCP_server_socket (int port)
{
	sock_ = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_ == -1)
	{
		throw server_error("Socket creating error");
	}	

	sock_info_.sin_family = AF_INET;
	sock_info_.sin_port = htons(port);
	sock_info_.sin_addr.s_addr = htons(INADDR_ANY);

	addr_ = (struct sockaddr*)&sock_info_;
	len_ = sizeof(sock_info_);
	if (bind(sock_, addr_, len_) == -1)
	{
		close(sock_);
		throw server_error("Connecting error");
	}
	
	listen(sock_, 1);

	client_ = accept(sock_, addr_, &len_);
	if (client_ == -1)
	{
		close(sock_);
		throw server_error("Accepting error");
	}
}


UDP_server_socket::UDP_server_socket (int port)
{
	sock_ = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_ == -1)
	{
		throw server_error("Socket creating error");
	}

	sock_info_.sin_family = AF_INET;
	sock_info_.sin_port = htons(port);
	sock_info_.sin_addr.s_addr = htons(INADDR_ANY);

	len_ = sizeof(sock_info_);

	addr_ = (struct sockaddr*)&sock_info_;
	if (bind(sock_, addr_, len_) == -1)
	{
		close(sock_);
		throw server_error("UDP connection error");
	}	
}

char * UDP_server_socket::sock_recieve()
{
	int msg_size, net_size;		
	if (recvfrom(sock_, &net_size, sizeof(net_size), 0, addr_, &len_) == -1)
		throw server_error("recieve error");
	msg_size = ntohl(net_size);
	char * msg = new char[msg_size];
	if (recvfrom(sock_, msg, msg_size, 0, addr_, &len_) == -1)
		throw server_error("recieve error");
	return msg;
}

