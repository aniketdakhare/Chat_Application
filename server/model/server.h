#pragma once
#include<iostream>
#include <sys/socket.h>
#include <netinet/in.h> 
#define PORT 9999

using namespace std;

class Server
{ 

    int serverSocket;
	struct sockaddr_in sreverAddress;
    int opt = 1;
    int addressLength = sizeof(sreverAddress);

    public:
        int getConnectionStatus();
        int getConnectedUsersSocket();
};