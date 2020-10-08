#pragma once
#include<iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define PORT 9999

using namespace std;

class User
{
    int userSocket;
    char buffer[1024];
    struct sockaddr_in serverAddress;

    public:
        int getConnectionStatus();
        int getUserSocket();
        void sendMessage(string, int);
        string receiveMessage(int);
};