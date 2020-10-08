#pragma once
#include<iostream>
#include <sys/socket.h>
#include <netinet/in.h> 
#define PORT 9999

using namespace std;

class User
{
    int userSocket;
    struct sockaddr_in serverAddress;

    public:
        int getConnectionStatus();
};