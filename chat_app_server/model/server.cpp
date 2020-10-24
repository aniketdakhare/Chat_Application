#include "server.h"

pthread_mutex_t Server::mutex = PTHREAD_MUTEX_INITIALIZER;
vector<ClientInfo> Server::registeredClients;
DBOperations Server::dbOperator;
Utility Server::util;

void Server::startServer()
{
	socklen_t clientAddressSize;

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	memset(serverAddress.sin_zero, '\0', sizeof(serverAddress.sin_zero));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientAddressSize = sizeof(clientAddress);

	bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

	listen(serverSocket, 5);
	cout << "Server Started.." << endl;

	while (1)
	{
		clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressSize);
		handleSession(clientSocket);
	}
}

void Server::handleSession(int clientSocket)
{
	pthread_mutex_lock(&mutex);
	inet_ntop(AF_INET, (struct sockaddr *)&clientAddress, ip, INET_ADDRSTRLEN);
	printf("%s connected\n", ip);
	client.mySocket = clientSocket;
	pthread_create(&receiveThread, NULL, receiveMessage, &client);
	pthread_mutex_unlock(&mutex);
}

void Server::sendMessage(string receiver, string message, int socket)
{
	pthread_mutex_lock(&mutex);

	for (int i = 0; i < registeredClients.size(); i++)
	{
		if (registeredClients[i].userId == receiver)
		{
			if (send(registeredClients[i].mySocket, message.c_str(), message.size(), 0) < 0)
			{
				cout << receiver << " is offline" << endl;
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
	string message;
	vector<string> msg;
	int counter = 1, length;

	bzero(sendMsg, sizeof(sendMsg));
	bzero(resvMsg, sizeof(resvMsg));

	while ((length = recv(client.mySocket, resvMsg, 500, 0)) > 0)
	{
		resvMsg[length] = '\0';
		msg = util.split(resvMsg, "#");

		if (msg[0] == "REGISTER")
		{
			registerUser(msg[1], msg[2], client.mySocket);
			memset(resvMsg, '\0', sizeof(resvMsg));
			continue;
		}

		if (msg[0] == "LOGIN")
		{
			loginUser(msg[1], msg[2], client);
			memset(resvMsg, '\0', sizeof(resvMsg));
			continue;
		}

		if (msg[1] == "$EXIT\n")
		{
			sleep(1);
			displayRegisteredUsers(client);
			continue;
		}

		if (msg[1] == "\n")
		{
			if (counter == 1)
			{
				message = loadMessages(client.userId, msg[0]);
				send(client.mySocket, message.c_str(), message.size(), 0);
				counter++;
			}
			continue;
		}

		dbOperator.storeClientMessages(client.userId, msg[0], msg[1]);
		message = "\n\x1B[93m" + client.userId + "\033[0m: \x1B[36m" + msg[1] + "\033[0m\n";
		sendMessage(msg[0], message, client.mySocket);
		memset(resvMsg, '\0', sizeof(resvMsg));
	}

	pthread_mutex_lock(&mutex);
	logout(client);
	pthread_mutex_unlock(&mutex);

	return 0;
}

string Server::loadMessages(string sender, string receiver)
{
	vector<pair<string, string>> messageDetails = dbOperator.getClientMessages(sender, receiver);
	string message;

	for (pair<string, string> msg : messageDetails)
	{
		if (msg.first == sender)
			message += msg.second;
		else
			message += "\x1B[33m" + msg.first + ":\033[0m \x1B[36m" + msg.second + "\033[0m\n";
	}

	return message;
}