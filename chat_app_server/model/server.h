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
        static pthread_mutex_t mutex;
        static vector<ClientInfo> registeredClients;

        static void sendMessage(string, string, int);
        static void *receiveMessage(void *sock);
        static void loginUser(string, string, ClientInfo&);
        static void registerUser(string, string, int);
        static bool getConnectedUserLoginStatus(string);
        static void logout(ClientInfo);
        static void displayRegisteredUsers(ClientInfo);
        static string loadMessages(string, string);

        void handleSession(int);    
   
    public:
        Server()
        {
            registeredClients = dbOperator.getRegisteredClientsList();
        }

        void startServer();
};