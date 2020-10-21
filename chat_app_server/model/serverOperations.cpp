#include "server.h"

bool Server::getConnectedUserLoginStatus(string userId)
{
	registeredClients = dbOperator.getRegisteredClientsList();
	bool status;

	for(ClientInfo client : registeredClients)
	{
		if(client.userId == userId)
		{
			status = client.loginStatus;
		}
	}
	return status;
}

void Server::registerUser(string userId, string password, int socket)
{
	string sendMsg;

	if (dbOperator.checkUserExists(userId) == true)
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

void Server::loginUser(string userId, string password, ClientInfo& cl)
{
	string sendMsg;
	if(getConnectedUserLoginStatus(userId))
	{
		sendMsg = "3";
		sleep(1);
		send(cl.mySocket, sendMsg.c_str(), sendMsg.size(), 0);
		return;
	}

	bool isUserLoggedIn = dbOperator.validateUser(userId, password);
	
	if(isUserLoggedIn)
	{
		sendMsg = "1";
		sleep(1);
		send(cl.mySocket, sendMsg.c_str(), sendMsg.size(), 0);
	}	
	else
	{
		sendMsg = "2";
		sleep(1);
		send(cl.mySocket, sendMsg.c_str(), sendMsg.size(), 0);
		return;
	}
	cl.userId = userId;
	cl.password = password;
	cl.loginStatus = true;
	dbOperator.updateOnlineStatus(cl);
	registeredClients = dbOperator.getRegisteredClientsList();
	displayRegisteredUsers(cl);
}

void Server::logout(ClientInfo cl)
{
	for(int i = 0; i < registeredClients.size(); i++) 
	{
		if(registeredClients[i].userId == cl.userId) 
		{
			cout << registeredClients[i].userId << " disconnected" << endl;
			cl.loginStatus = false;
			cl.mySocket = 1;
			dbOperator.updateOnlineStatus(cl);
			registeredClients = dbOperator.getRegisteredClientsList();
		}
	}
}

void Server::displayRegisteredUsers(ClientInfo cl)
{
	registeredClients = dbOperator.getRegisteredClientsList();
	string onlineClients = "\n\x1B[93mRegistered Users\033[0m\n\n";

	for(int i = 1; i <= registeredClients.size(); i++)
	{
		if(registeredClients[i - 1].userId != cl.userId)
		{
			if(registeredClients[i - 1].loginStatus == true)
			{
				onlineClients += to_string(i) +": \x1B[32m"+ registeredClients[i - 1].userId + "\033[0m\n";
			}
			else
			{
				onlineClients += to_string(i) +": "+ registeredClients[i - 1].userId + "\n";
			}	
		}	
		else
		{
			onlineClients += to_string(i) +": \x1B[35m"+ registeredClients[i - 1].userId + " (You)\033[0m\n";
		}	
	}

	onlineClients += "\n\x1B[33mSelect userId to chat with: \033[0m";
	send(cl.mySocket, onlineClients.c_str(), onlineClients.size(), 0);
}