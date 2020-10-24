#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "utility.h"

#define PORT 9090

using namespace std;

class Client
{
	char ip[INET_ADDRSTRLEN];
	int mySocket;
	pthread_t receiveThread;
	struct sockaddr_in receiverAddress;
	static char message[500];
	static string receiverClient;
	static string myUserId;
	static pthread_mutex_t mutex;

	void login();
	void registerClient();
	void displayOptions();
	void sendMessage();
	void selectClientToChat();
	void displayMenu();
	static void *receiveMessage(void *);

public:
	void startUser();
};