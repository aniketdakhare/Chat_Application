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