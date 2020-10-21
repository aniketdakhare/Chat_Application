#include "server.h"

vector<int> Server::clients;
pthread_mutex_t  Server::mutex = PTHREAD_MUTEX_INITIALIZER;
vector<ClientInfo> Server::connectedClients;
DBOperations Server::dbOperator;
Utility Server::util;

void Server::sendMessage(char *message, int socket)
{
	pthread_mutex_lock(&mutex);
	for(int i = 0; i < clients.size(); i++) 
    {
		if(clients[i] != socket)
        {
			if(send(clients[i], message, strlen(message), 0) < 0) 
            {
				perror("sending failure");
				continue;
			}
		}
	}
	pthread_mutex_unlock(&mutex);	
}

void *Server::receiveMessage(void *sock)
{
	struct ClientInfo client = *((struct ClientInfo *)sock);
	char resvMsg[500];
	char sendMsg[500];
	vector<string> msg;
	int length;

	bzero(sendMsg, sizeof(sendMsg));
	bzero(resvMsg, sizeof(resvMsg));

	while((length = recv(client.mySocket, resvMsg, 500, 0)) > 0) 
	{
		resvMsg[length] = '\0';
		msg = util.split(resvMsg, "#");

		if (msg[0] == "REGISTER")
		{	
			registerUser(msg[1], msg[2], client.mySocket);
			memset(resvMsg,'\0',sizeof(resvMsg));	
			continue;
		}

		if (msg[0] == "LOGIN")
		{
			loginUser(msg[1], msg[2], client.mySocket);
			memset(resvMsg,'\0',sizeof(resvMsg));
			continue;
		}
		sendMessage(resvMsg, client.mySocket);
		memset(resvMsg,'\0',sizeof(resvMsg));
	}
	
	pthread_mutex_lock(&mutex);
	logout(client);
	pthread_mutex_unlock(&mutex);
	
	return 0;
}

void Server::startServer()
{
	int clientCounter = 0;
	socklen_t clientAddressSize;

	serverSocket = socket(AF_INET,SOCK_STREAM,0);
	memset(serverAddress.sin_zero,'\0',sizeof(serverAddress.sin_zero));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientAddressSize = sizeof(clientAddress);

	bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

	listen(serverSocket, 1);
	cout << "Server Started.." << endl;
	
	while(1) 
	{
		if(clientCounter < 2)
		{
			clientSocket = accept(serverSocket,(struct sockaddr *)&clientAddress,&clientAddressSize);
			handleSession(clientSocket);
			clientCounter++;
		}
	}
}

void Server::handleSession(int clientSocket)
{
	pthread_mutex_lock(&mutex);
	inet_ntop(AF_INET, (struct sockaddr *)&clientAddress, ip, INET_ADDRSTRLEN);
	printf("%s connected\n",ip);
	client.mySocket = clientSocket;
	strcpy(client.ip, ip);
	clients.push_back(clientSocket);
	pthread_create(&receiveThread, NULL, receiveMessage, &client);
	pthread_mutex_unlock(&mutex);
}