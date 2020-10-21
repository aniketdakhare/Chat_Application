#pragma once
#include <string>
#include <netinet/in.h> 

using namespace std;

struct ClientInfo 
{
	int mySocket;
    int receiverClientSocket;
    string userId;
    string password;
    bool loginStatus = false;
	char ip[INET_ADDRSTRLEN];
};