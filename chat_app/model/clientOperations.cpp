#include "client.h"

void Client::displayOptions()
{
	int choice;
	cout << "\n\t\x1b[1m\x1B[93mWELCOME TO CHAT_APP\033[0m\x1b[0m\n\nPlease select the option.\n\n1: Register\n2: login\n";
	cin >> choice;

	switch (choice)
	{
	case 1:
	{
		registerClient();
		login();
	}
	break;
	case 2:
		login();
		break;
	}
}

void Client::registerClient()
{
	string userId, password, sendMsg;
	char msg[500];
	system("clear");
	cout << "\n\t\x1b[1m\x1B[93mRegister to CHAT_APP\033[0m\x1b[0m" << endl;
	while (true)
	{
		cout << "\n\x1B[36mEnter UserId : \033[0m";
		cin >> userId;
		cout << "\n\x1B[36mEnter Password : \033[0m";
		cin >> password;

		sendMsg = "REGISTER#" + userId + "#" + password;

	registerAgain:
		write(mySocket, sendMsg.c_str(), sendMsg.size());
		sendMsg.clear();
		memset(msg, '\0', sizeof(msg));
		recv(mySocket, msg, 500, 0);

		if (msg[0] == '\n')
			goto registerAgain;

		if (msg[0] == 'N')
		{
			cout << "\n\x1B[31mUser is already registered\033[0m\n\x1B[33mPlease change your UserId or Password.\n\033[0m" << endl;
			continue;
		}

		if (msg[0] == 'Y')
			break;
	}
}

void Client::login()
{
	string userId, password, msg;
	system("clear");
	cout << "\n\t\x1b[1m\x1B[93mLogin to CHAT_APP\033[0m\x1b[0m" << endl;

	while (true)
	{
		cout << "\n\x1B[36mEnter UserId : \033[0m";
		cin >> userId;
		cout << "\n\x1B[36mEnter Password : \033[0m";
		cin >> password;

		msg = "LOGIN#" + userId + "#" + password;

	dothis:
		write(mySocket, msg.c_str(), msg.size());
		msg.clear();
		memset(message, '\0', sizeof(message));
		recv(mySocket, message, 500, 0);

		if (message[0] == '\n')
			goto dothis;

		if (message[0] == '1')
		{
			system("clear");
			cout << "\n\x1B[32mLogin Successfully...\033[0m" << endl;
			break;
		}
		else if (message[0] == '2')
		{
			cout << "\n\x1B[31mInvalid UserId or Password\033[0m" << endl;
		}
		else if (message[0] == '3')
		{
			cout << "\n\x1B[31mGiven user is already logged in...!!!\033[0m" << endl;
		}
	}

	displayMenu();
}

void Client::selectClientToChat()
{
	memset(message, '\0', sizeof(message));
	recv(mySocket, message, 500, 0);
	cout << message;
	cin >> receiverClient;
	system("clear");
}

void Client::displayMenu()
{
	int choice;
	bool flag = true;

	while (flag)
	{
		cout << "\n\x1B[33mSelect Your Option: \033[0m\n\n1: Select UserID to chat\n2: Logout\n\n";
		cin >> choice;

		switch (choice)
		{
		case 1:
			flag = false;
			selectClientToChat();
			break;
		case 2:
			exit(0);
			break;
		default:
			cout << "\n\x1B[31mInvalid Choice..\033[0m" << endl;
		}
	}
}