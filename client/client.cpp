#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>

using namespace std;


void udp_connect(int port)
{
	int client;
	client = socket(AF_INET, SOCK_DGRAM, 0);
	if (client == -1)
	{
		exit(1);
	}

	struct sockaddr_in client_sock;
	client_sock.sin_family = AF_INET;
    client_sock.sin_port = htons(port);
    client_sock.sin_addr.s_addr = htons(INADDR_ANY);

	cout << "Send \"quit\" to quit" << endl;

	socklen_t s_len = sizeof(client_sock);

	string buffer;
	int sent, recieved, msg_size, net_size;
	while(1)
	{
		sent = 0;
		recieved = 0;
		getline(cin, buffer);
		msg_size = buffer.size() + 1;
		net_size = htonl(msg_size);

		sendto(client, (const char *)&net_size, sizeof(net_size), 0, (struct sockaddr *)&client_sock, s_len);

		
			while (sent < msg_size)
			{
				sent += sendto(client, &buffer[0] + sent, msg_size - sent, 0, (struct sockaddr *)&client_sock, s_len);	
			}

			if (buffer == "quit")
			{
				cout << "Connection closed" << endl;
				break;
			}	

			char * rec_msg = new char[msg_size];
			char rec_buf[65560]; 
			while (recieved < msg_size)
			{
				recieved += recvfrom(client, rec_buf, msg_size - recieved, 0, (struct sockaddr *)&client_sock, &s_len);
				strcpy(rec_msg, rec_buf);
			}
			cout << rec_msg << endl;
			delete[] rec_msg;
	}

	close(client);
}

void tcp_connect(int port, int client)
{
	cout << "Send \"quit\" to quit" << endl;
	
	string buffer;
	int msg_size, net_size;

	while(1)
	{
		getline(cin, buffer);
		msg_size = buffer.size() + 1;
		net_size = htonl(msg_size);
		send(client, (const char *)&net_size, sizeof(net_size), 0);
		send(client, &buffer[0], msg_size, 0);
		if (buffer == "quit")
		{
			cout << "Connection closed" << endl;
			break;
		}
		
		char * rec_msg = new char[msg_size];
		recv(client, rec_msg, msg_size, 0);
		cout << rec_msg << endl;
		delete[] rec_msg;
	}

}


int main(int argc, char * argv[])
{
	int client;
	int port = 2046;

	if (argc < 1)
	{
		cout << "Wrong input" << endl;
		exit(1);
	}

	client = socket(AF_INET, SOCK_STREAM, 0);
	if (client == -1)
	{
		exit(1);
	}

	struct sockaddr_in client_sock;
	client_sock.sin_family = AF_INET;
    client_sock.sin_port = htons(port);
    client_sock.sin_addr.s_addr = htons(INADDR_ANY);

	struct sockaddr * client_addr = (struct sockaddr*)&client_sock;
	if (connect(client, client_addr, sizeof(client_sock)) == -1)
	{
		cout << "Connection error" << endl;
		close(client);
		exit(1);
	}	

	if (argv[1] == string("-u") || argv[1] == string("--udp"))
	{
		send(client, "u", 2, 0);
		close(client);
		udp_connect(port);
	}
	else if (argv[1] == string("-t") || argv[1] == string("--tcp"))
	{
		send(client, "t", 2, 0);
		tcp_connect(port, client);
		close(client);
	}
	else
	{
		cout << "Wrong arguments" << endl;
		close(client);
		exit(1);
	}
	
	return 0;
}
