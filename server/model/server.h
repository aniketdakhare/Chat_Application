#pragma once
#include<iostream>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <vector>
#include "user.h"
#include "../DBOperations/DBOperations.h"

#define PORT 9090

using namespace std;

class Server
{ 
    ClientInfo client;
	struct sockaddr_in serverAddress, clientAddress;
	pthread_t receiveThread;
	char ip[INET_ADDRSTRLEN];
  
    static DBOperations dbOperator;
    static int clients[1];
	static char message[500];
    static int numberOfConnectedClients;
    static pthread_mutex_t mutex;
    static vector<ClientInfo> connectedClients;

    static void sendMessage(char*, int);
    static void *receiveMessage(void *sock);

    void loginUser(int, char*);
    void displayClintConsole(int, char*);
    void registerUser(int);
    bool getConnectUserLoginStatus(string, string);
    void handleSession(int);
    pair<string, string> getUserCredentials(int);
    
    public:
        void startServer();
};