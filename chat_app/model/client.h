#pragma once
#include<iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 9090

using namespace std;

class Client
{
	char ip[INET_ADDRSTRLEN];
	int mySocket;
	pthread_t receiveThread;	
	struct sockaddr_in receiverAddress;
	static char message[500];

	void login();
	void registerClient();
	void displayOptions();
    void sendMessage();
    static void* receiveMessage(void*);

    public:
        void startUser();       
};