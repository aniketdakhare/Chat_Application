#include "server.h"

int Server::numberOfConnectedClients = 0;
int Server::clients[1];
char Server::message[500];
pthread_mutex_t  Server::mutex = PTHREAD_MUTEX_INITIALIZER;
vector<ClientInfo> Server::connectedClients;
DBOperations Server::dbOperator;

void Server::sendMessage(char *message, int socket)
{
	int i;
	pthread_mutex_lock(&mutex);
	for(i = 0; i < numberOfConnectedClients; i++) 
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
	char message[500];
	int length;

	while((length = recv(client.mySocket, message, 500, 0)) > 0) 
	{
		message[length] = '\0';
		sendMessage(message, client.mySocket);
		memset(message,'\0', sizeof(message));
	}

	pthread_mutex_lock(&mutex);
	printf("%s disconnected\n", client.ip);
	
	for(int i = 0; i < numberOfConnectedClients; i++) 
	{
		if(clients[i] == client.mySocket) 
		{
			int j = i;
			connectedClients.erase(connectedClients.begin() + i);

			while(j < numberOfConnectedClients - 1) 
			{
				clients[j] = clients[j + 1];
				j++;
			}
		}
	}

	numberOfConnectedClients--;
	pthread_mutex_unlock(&mutex);
	
	return 0;
}

void Server::startServer()
{
	int clientSocket, serverSocket; 
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
	displayClintConsole(clientSocket, ip);
	numberOfConnectedClients++;
	pthread_create(&receiveThread, NULL, receiveMessage, &client);
	pthread_mutex_unlock(&mutex);
}

void Server::loginUser(int socket, char* ip)
{
    bool flag = true;
	int length = 0;
	char message[100];
	int receiverSocket;
	pair<string, string> logintDetails;

    while(flag)
    {
		sleep(1);
		logintDetails = getUserCredentials(socket);

		if(getConnectUserLoginStatus(logintDetails.first, logintDetails.second))
		{
			strcpy(message, "3");
			send(socket, message, strlen(message),0);
			sleep(1);
			continue;
		}

		bool isUserLoggedIn = dbOperator.validateUser(logintDetails.first, logintDetails.second);
		
		if(isUserLoggedIn)
		{
			flag = false;
			strcpy(message, "1");
			send(socket, message, strlen(message),0);
		}	
        else
		{
			strcpy(message, "2");
			send(socket, message, strlen(message),0);
		}
    }

	client.mySocket = socket;
	strcpy(client.ip, ip);
	client.userId = logintDetails.first;
	client.password = logintDetails.second;
	client.loginStatus = true;
	clients[numberOfConnectedClients] = socket;
	connectedClients.push_back(client);
}

bool Server::getConnectUserLoginStatus(string userId, string password)
{
	for(ClientInfo client : connectedClients)
	{
		if(client.userId == userId && client.password == password)
		{
			return true;
		}
	}
	return false;
}

void Server::displayClintConsole(int socket, char* ip)
{
	memset(message,'\0',sizeof(message));
	strcpy(message, "\n\t\x1b[1m\x1B[93mWELCOME TO CHAT_APP\033[0m\x1b[0m\n\nPlease select the option.\n\n1: Register\n2: login\n");
	sleep(1);
	send(socket, message, strlen(message),0);
	memset(message,'\0',sizeof(message));
	int length = recv(socket, message, 500, 0);
	message[length] = '\0';
	
	switch (message[0])
	{
		case '1':
			{
				registerUser(socket);
				loginUser(socket, ip);
			}
			break;
		case '2':
			loginUser(socket, ip);
			break;
	}
}

void Server::registerUser(int clientSocket)
{
	pair<string, string> registrationDetails;

	while (true)
	{
		sleep(1);
		registrationDetails = getUserCredentials(clientSocket);
		
		if (dbOperator.validateUser(registrationDetails.first, registrationDetails.second) == true)
		{
			memset(message,'\0',sizeof(message));
			strcpy(message, "N");
			send(clientSocket, message, strlen(message), 0);
		}
		else
		{
			memset(message,'\0',sizeof(message));
			strcpy(message, "Y");
			send(clientSocket, message, strlen(message), 0);
			break;
		}
	}
	
	dbOperator.registerUser(registrationDetails.first, registrationDetails.second);
}

pair<string, string> Server::getUserCredentials(int socket)
{
	int length;
	pair<string, string> credentials;

	memset(message,'\0',sizeof(message));
	strcpy(message, "\n\x1B[36mEnter UserId : \033[0m");
	send(socket, message, strlen(message),0);
	memset(message,'\0',sizeof(message));
	length = recv(socket, message, 500, 0);
	message[length] = '\0';
	credentials.first= message;

	memset(message,'\0',sizeof(message));
	strcpy(message, "\n\x1B[36mEnter Password : \033[0m");
	send(socket, message,strlen(message),0);
	memset(message,'\0',sizeof(message));
	length = recv(socket, message, 500, 0);
	message[length] = '\0';
	credentials.second = message;

	return credentials;
}