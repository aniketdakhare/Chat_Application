#pragma once
#include<iostream>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <vector>
#include "user.h"

#define PORT 8888

using namespace std;

class Server
{ 
    ClientInfo client;
	struct sockaddr_in serverAddress, clientAddress;
	pthread_t receiveThread;
	char ip[INET_ADDRSTRLEN];
  
    static int clients[1];
	static char message[500];
    static int numberOfConnectedClients;
    static pthread_mutex_t mutex;

    static void sendMessage(char*,int);
    static void *receiveMessage(void *sock);
    static bool getConnectUserLoginStatus(string, string);

    void handleSession(int);
    
    public:
        void startServer();
};