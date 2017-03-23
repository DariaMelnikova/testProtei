#include "client_socket.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>

using namespace std;

TCP_client_socket::TCP_client_socket(int port)
{
	sock_ = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_ == -1)
	{
		throw client_error("Socket creating error");
	}

	sock_info_.sin_family = AF_INET;
	sock_info_.sin_port = htons(port);
	sock_info_.sin_addr.s_addr = htons(INADDR_ANY);

	len_ = sizeof(sock_info_);
	addr_ = (struct sockaddr*)&sock_info_;

	if (connect(sock_, addr_, len_) == -1)
	{
		close(sock_);
		throw client_error("Connecting error");
	}	
}

void TCP_client_socket::sock_send (string msg)
{
	int msg_size, net_size;
	msg_size = msg.size() + 1;
	net_size = htonl(msg_size);
	if (send(sock_, (const char *)&net_size, sizeof(net_size), 0) == -1 || 	send(sock_, &msg[0], msg_size, 0) == -1)
		throw client_error("Can't send your massage");
}

UDP_client_socket::UDP_client_socket(int port)
{
	sock_ = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_ == -1)
	{
		throw client_error("Socket creating error");
	}

	sock_info_.sin_family = AF_INET;
	sock_info_.sin_port = htons(port);
	sock_info_.sin_addr.s_addr = htons(INADDR_ANY);

	len_ = sizeof(sock_info_);
	addr_ = (struct sockaddr *)&sock_info_;
}

void UDP_client_socket::sock_send(string msg)
{
	int msg_size, net_size;
	msg_size = msg.size() + 1;
	net_size = htonl(msg_size);

	if (sendto(sock_, (const char *)&net_size, sizeof(net_size), 0, addr_, len_) == -1 || sendto(sock_, &msg[0], msg.size() + 1, 0, addr_, len_) == -1)
		throw client_error("Can't send your message");
}


