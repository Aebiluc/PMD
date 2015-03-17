#include<iostream>
#include<string>
#include <winsock2.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

void reception(SOCKET sock)
{
	static char buffer[255];
	while (1)
	{
		recv(sock, buffer, sizeof(buffer), 0);
		cout << buffer << endl;
	}
}


int main()
{
	WSADATA WSAData;
	SOCKET sock;
	SOCKADDR_IN sin;
	string* test;
	
	static char buffer[255];
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	/* Tout est configuré pour se connecter sur IRC, haarlem, Undernet. */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	sin.sin_addr.s_addr = inet_addr("192.168.0.7");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(9091);

	cout << "Tentativer de connexion" << endl;
	int ret = connect(sock, (SOCKADDR *)&sin, sizeof(sin));
	if (ret < 0){
		cout << "Erreur de connexion" << endl;
		system("PAUSE");
		exit(1);
	}

	thread t1(reception, sock);

	test = new string(buffer);
	cout << *test << endl;

	string message;
	while (message != "exit"){

		cin >> message;
		send(sock, message.c_str(), 255, 0);
		
	}

	closesocket(sock);
	WSACleanup();
	system("PAUSE");
	return 0;
}