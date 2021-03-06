#include "client.h"

string Client::receiverClient;
char Client::message[500];
pthread_mutex_t Client::mutex;
string Client::myUserId;

void Client::startUser()
{
	mySocket = socket(AF_INET, SOCK_STREAM, 0);

	memset(receiverAddress.sin_zero, '\0', sizeof(receiverAddress.sin_zero));
	receiverAddress.sin_family = AF_INET;
	receiverAddress.sin_port = htons(PORT);
	receiverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

	connect(mySocket, (struct sockaddr *)&receiverAddress, sizeof(receiverAddress));

	displayOptions();
	cout << "\n\033[3;42;30mONLINE - " << myUserId << "\033[0m\t\t\n";
	sendMessage();
}

void Client::sendMessage()
{
	string sendMsg;
	vector<string> resvMsg;
	pthread_create(&receiveThread, NULL, receiveMessage, &mySocket);

	while (fgets(message, 500, stdin) > 0)
	{
		message[strlen(message)] = '\0';
		resvMsg = Utility::split(message, "-");

		if(strstr(message, "$LOGOUT"))
			exit(0);

		if (resvMsg.at(0)[0] == '@')
		{
			receiverClient = resvMsg.at(0).substr(1, resvMsg.at(0).size() - 1);
			sendMsg = receiverClient + "#" + resvMsg[1];
		}
		else
			sendMsg = receiverClient + "#" + message;

		if (write(mySocket, sendMsg.c_str(), sendMsg.size()) < 0)
		{
			perror("message not sent");
			exit(1);
		}

		memset(message, '\0', sizeof(message));
	}

	pthread_join(receiveThread, NULL);
	close(mySocket);
}

void *Client::receiveMessage(void *socket)
{
	int receiverSocket = *((int *)socket);
	char message[500];
	int len;

	while ((len = recv(receiverSocket, message, sizeof(message), 0)) > 0)
	{
		message[len - 1] = '\0';
		if (strstr(message, "Registered"))
		{
			system("clear");
			cout << message;
		}
		else
			cout << message << endl;

		memset(message, '\0', sizeof(message));
	}
	return 0;
}