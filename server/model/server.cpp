#include "server.h"

int Server::getConnectionStatus()
{
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEPORT , &opt, sizeof(opt));

    sreverAddress.sin_family = AF_INET;
    sreverAddress.sin_port = htons(PORT);
    sreverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr*) &sreverAddress, addressLength);

    int usersCount = listen(serverSocket, 3) > 0;
    
    return usersCount;
}

int Server::getConnectedUsersSocket()
{
    int userSocket = accept(serverSocket, (struct sockaddr*) &sreverAddress, (socklen_t*) &addressLength);

    return userSocket;
}

void Server::sendMessage(string message, int socket)
{
    memset(buffer, 0, sizeof(message));
    strcpy(buffer, message.c_str());
	send(socket, buffer, strlen(buffer), 0); 	
}

string Server::receiveMessage(int socket)
{
    memset(buffer, 0, sizeof(buffer));
    recv(socket, (char*)&buffer, sizeof(buffer), 0);
    return buffer;
}