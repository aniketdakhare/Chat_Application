#pragma once
#include<iostream>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <vector>
#include "user.h"
#include "utility.h"
#include "../DBOperations/DBOperations.h"

#define PORT 9090

using namespace std;

class Server
{ 
    protected:
    
    ClientInfo client;
	sockaddr_in serverAddress, clientAddress;
	pthread_t receiveThread;

	char ip[INET_ADDRSTRLEN];
    int clientSocket, serverSocket;
  
    static Utility util;
    static DBOperations dbOperator;
    static vector<int> clients;
    static pthread_mutex_t mutex;
    static vector<ClientInfo> connectedClients;

    static void sendMessage(char*, int);
    static void *receiveMessage(void *sock);
    static void loginUser(string, string, int);
    static void registerUser(string, string, int);
    static bool getConnectUserLoginStatus(string, string);
    static void logout(ClientInfo);
        
    void handleSession(int);    

    public:
        void startServer();
};