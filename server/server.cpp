#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include "server_socket.hpp"
#include "text_handlers.hpp"
using namespace std;


int main()
{

	int port = 2046;
	char * mode;

	Server_socket * sock = new TCP_server_socket(port);
	
	try 
	{
		mode = sock->sock_recieve();
	}
	catch (server_error const& err)
	{
		sock->sock_close();
		exit(1);
	}

	if(mode == string("u"))
	{
		sock->sock_close();
		delete sock;
		sock = new UDP_server_socket(port);
	}

	cout << "Client connected." << endl;

	while(1)
	{
		char * buffer = sock->sock_recieve();

		if (string(buffer) == "quit")
		{
			cout << "Connection closed" << endl;
			break;
		}

		handler(string(buffer));

		cout << buffer << endl;

		sock->sock_send(string(buffer));
		
		delete[] buffer;
	}


	sock->sock_close();
	delete[] mode;	
	delete sock;
	return 0;
}
