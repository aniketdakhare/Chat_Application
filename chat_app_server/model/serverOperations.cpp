#include "server.h"

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

void Server::registerUser(string userId, string password, int socket)
{
	string sendMsg;

	if (dbOperator.validateUser(userId, password) == true)
	{
		sendMsg = "N";
		sleep(1);
		send(socket, sendMsg.c_str(), sendMsg.size(), 0);
		return;
	}
	else
	{
		sendMsg = "Y";
		sleep(1);
		send(socket, sendMsg.c_str(), sendMsg.size(), 0);
		dbOperator.registerUser(userId, password);
	}
}

void Server::loginUser(string userId, string password, int socket)
{
	string sendMsg;
	if(getConnectUserLoginStatus(userId, password))
	{
		sendMsg = "3";
		sleep(1);
		send(socket, sendMsg.c_str(), sendMsg.size(), 0);
		return;
	}

	bool isUserLoggedIn = dbOperator.validateUser(userId, password);
	
	if(isUserLoggedIn)
	{
		sendMsg = "1";
		sleep(1);
		send(socket, sendMsg.c_str(), sendMsg.size(), 0);
	}	
	else
	{
		sendMsg = "2";
		sleep(1);
		send(socket, sendMsg.c_str(), sendMsg.size(), 0);
		return;
	}
	ClientInfo cl;
	cl.userId = userId;
	cl.password = password;
	cl.mySocket = socket;
	cl.loginStatus = true;
	connectedClients.push_back(cl);
}

void Server::logout(ClientInfo client)
{
	for(int i = 0; i < clients.size(); i++) 
	{
		if(clients[i] == client.mySocket) 
		{
			cout << connectedClients[i].userId << " disconnected" << endl;
			clients.erase(clients.begin() + i);
			connectedClients.erase(connectedClients.begin() + i);
		}
	}
}