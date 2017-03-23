#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include "client_socket.hpp"


using namespace std;



int main(int argc, char * argv[])
{
	int port = 2046;

	if (argc < 2)
	{
		throw client_error("Not enough arguments");
	}

	Client_socket * sock = new TCP_client_socket(port);

	if (argv[1] == string("-u") || argv[1] == string("--udp"))
	{
		sock->sock_send("u");
		sock->sock_close();
		delete sock;
		sock = new UDP_client_socket(port);
	}
	else if (argv[1] == string("-t") || argv[1] == string("--tcp"))
	{
		sock->sock_send("t");
	}
	else
	{
		sock->sock_close();
		throw client_error("Wrong arguments");
	}

	cout << "You are connected now. Send \"quit\" to quit" << endl;
	
	string buffer;

	while(1)
	{
		char * rec_msg;
		getline(cin, buffer);
		sock->sock_send(buffer);

		if (buffer == "quit")
		{
			cout << "Connection closed" << endl;
			break;
		}

		try 
		{
			rec_msg = new char[buffer.size() + 1];
			sock->sock_recieve(rec_msg, buffer.size() + 1);
			cout << rec_msg << endl;
			delete[] rec_msg;
		} 
		catch (client_error const& err)
		{
			delete[] rec_msg;
		}
	}

	sock->sock_close();
	delete sock;
	return 0;
}
