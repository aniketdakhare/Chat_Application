#include "user.h"

int User::getConnectionStatus()
{
    userSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    serverAddress.sin_family = AF_INET; 
	serverAddress.sin_port = htons(PORT); 
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    int connectionStatus = connect(userSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    return connectionStatus;
}

int User::getUserSocket()
{
    return this->userSocket;
}

void User::sendMessage(string message, int socket)
{
    memset(buffer, 0, sizeof(message));
    strcpy(buffer, message.c_str());
	send(socket, buffer, strlen(buffer), 0); 	
}

string User::receiveMessage(int socket)
{
    memset(buffer, 0, sizeof(buffer));
    recv(socket, (char*)&buffer, sizeof(buffer), 0);
    return buffer;
}