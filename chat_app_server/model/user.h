#pragma once
#include <string>
#include <netinet/in.h> 

using namespace std;

struct ClientInfo 
{
	int mySocket;
    string userId;
    string password;
    bool loginStatus = false;
};