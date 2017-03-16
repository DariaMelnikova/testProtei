#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include "text_handlers.hpp"

using namespace std;


int create_socket(int mode)
{
	int client = socket(AF_INET, mode, 0);
	if (client == -1)
	{
		cout << "Socket creating error" << endl;
		exit(1);
	}	
	return client;
}


void tcp_server_connection(int server, int client)
{
	cout << "Client connected." << endl;

	int msg_size, net_size;	

	while(1)
	{
		recv(server, &net_size, sizeof(net_size), 0);
		msg_size = ntohl(net_size);
		char * buffer = new char[msg_size];

		recv(server, buffer, msg_size, 0);

		string str_buf = string(buffer);

		if (str_buf == "quit")
		{
			cout << "Connection cosed" << endl;
			break;
		}

		map<int, int> d_map = handler(&str_buf);

		cout << "Sum of the digits: ";		
		cout << get_sum(d_map) << endl;
		cout << "All digits in desc order: ";
		show_all(d_map);
		cout << "Min: ";
		cout << show_min(d_map) << endl;
		cout << "Max: ";
		cout << show_max(d_map) << endl;

		send(server, buffer, msg_size, 0);
		
		delete[] buffer;
	}

	close(server);
	close(client);
}

void udp_server_connection(int port)
{
	int client = create_socket(SOCK_DGRAM);
	struct sockaddr_in server_sock;
	server_sock.sin_family = AF_INET;
	server_sock.sin_port = htons(port);
	server_sock.sin_addr.s_addr = htons(INADDR_ANY);

	socklen_t s_len = sizeof(server_sock);

	struct sockaddr * server_addr = (struct sockaddr*)&server_sock;
	if (bind(client, server_addr, s_len) == -1)
	{
		cout << "UDP Connection error" << endl;
		close(client);
		exit(1);
	}

	cout << "Client connected" << endl;	

	int msg_size, net_size, recieved, sent;

	while(1)
	{
		sent = 0;
		recieved = 0;

		recvfrom(client, &net_size, sizeof(net_size), 0, server_addr, &s_len);	
		msg_size = ntohl(net_size);
		char * message = new char[msg_size];
		char buff[65560]; 

		while (recieved < msg_size)
		{
			recieved += recvfrom(client, buff, msg_size - recieved, 0, server_addr, &s_len);
			strcpy(message, buff);
		}
		cout << message << endl;

		string str_buf = string(message);

		if (str_buf == "quit")
		{
			cout << "Connection cosed" << endl;
			break;
		}

		map<int, int> d_map = handler(&str_buf);

		cout << "Sum of the digits: ";		
		cout << get_sum(d_map) << endl;
		cout << "All digits in desc order: ";
		show_all(d_map);
		cout << "Min: ";
		cout << show_min(d_map) << endl;
		cout << "Max: ";
		cout << show_max(d_map) << endl;

		while (sent < msg_size)
		{
			sent += sendto(client, message + sent, msg_size - sent, 0, server_addr, s_len);
		}

		delete[] message;

	}

	close(client);

}

int main()
{

	int port = 2046;
	int server, client;

	client = create_socket(SOCK_STREAM);

	struct sockaddr_in server_sock;
	server_sock.sin_family = AF_INET;
	server_sock.sin_port = htons(port);
	server_sock.sin_addr.s_addr = htons(INADDR_ANY);

	struct sockaddr * server_addr = (struct sockaddr*)&server_sock;
	if (bind(client, server_addr, sizeof(server_sock)) == -1)
	{
		cout << "Connection error" << endl;
		close(client);
		exit(1);
	}
	
	listen(client, 1);

	socklen_t addr_size = sizeof(server_sock);
	server = accept(client, server_addr, &addr_size);
	if (server == -1)
	{
		cout << "Accepting error" << endl;
		close(client);
		exit(1);
	}

	char mode[2];
	recv(server, mode, 2, 0);
	if(mode == string("t"))
		tcp_server_connection(server, client);
	if(mode == string("u"))
	{
		close(server);
		close(client);
		udp_server_connection(port);
	}

	return 0;
}
