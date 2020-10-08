#pragma once
#include<iostream>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <string.h>
#include <unistd.h>
#define PORT 9999

using namespace std;

class Server
{ 

    int serverSocket;
	struct sockaddr_in sreverAddress;
    int opt = 1;
    char buffer[1024];
    int addressLength = sizeof(sreverAddress);

    public:
        int getConnectionStatus();
        int getConnectedUsersSocket();
        void sendMessage(string, int);
        string receiveMessage(int);
};