#include "client.h"

char Client::message[500];

void Client::startUser()
{
	mySocket = socket(AF_INET, SOCK_STREAM, 0);

	memset(receiverAddress.sin_zero, '\0', sizeof(receiverAddress.sin_zero));
	receiverAddress.sin_family = AF_INET;
	receiverAddress.sin_port = htons(PORT);
	receiverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(mySocket, (struct sockaddr *)&receiverAddress, sizeof(receiverAddress));

	displayOptions();
    printf("\n\033[3;42;30mONLINE\033[0m\t\t\n");
	sendMessage();
}

void Client::sendMessage()
{
	pthread_create(&receiveThread, NULL, receiveMessage, &mySocket);

	while(fgets(message, 500, stdin) > 0) 
    {
		if(write(mySocket, message, strlen(message)) < 0) 
        {
			perror("message not sent");
			exit(1);
		}
		memset(message, '\0', sizeof(message));
	}
	
    pthread_join(receiveThread, NULL);
	close(mySocket);
}

void *Client::receiveMessage(void* socket)
{
	int receiverSocket = *((int *)socket);
	char message[500];
	int len;
	
    while((len = recv(receiverSocket, message, sizeof(message), 0)) > 0) 
    {
		message[len] = '\0';
		cout << message;
		memset(message, '\0', sizeof(message));
	}
}

void Client::login()
{
	system("clear");
	cout << "\n\t\x1b[1m\x1B[93mLogin to CHAT_APP\033[0m\x1b[0m" << endl;

	while(true)
	{
		memset(message, '\0', sizeof(message));
		recv(mySocket, message, 500, 0);
		if(message[0] == '1')
		{
			system("clear");
			cout << "\n\x1B[32mLogin Successfully...\033[0m" << endl;
			break;
		}
		else if(message[0] == '2')
		{
			cout << "\n\x1B[31mInvalid UserId or Password\033[0m" << endl;
			continue;
		}
		else if(message[0] == '3')
		{
			cout << "\n\x1B[31mGiven user is already logged in...!!!\033[0m" << endl;
			continue;
		}
		cout << message;
		memset(message, '\0', sizeof(message));
		cin >> message;
		write(mySocket, message, strlen(message));
	}	
}

void Client::displayOptions()
{
	memset(message, '\0', sizeof(message));
	recv(mySocket, message, 500, 0);
	cout << message;
	memset(message, '\0', sizeof(message));
	cin >> message;
	write(mySocket, message, strlen(message));

	switch (message[0])
	{
		case '1':
			{
				registerClient();
				login();
			}
			break;
		case '2':
			login();
			break;
	}
}

void Client::registerClient()
{
	system("clear");
	cout << "\n\t\x1b[1m\x1B[93mRegister to CHAT_APP\033[0m\x1b[0m" << endl;
	while (true)
	{
		for (int i = 0; i < 2; i++)
		{
			memset(message, '\0', sizeof(message));
			recv(mySocket, message, 500, 0);
			cout << message;
			memset(message, '\0', sizeof(message));
			cin >> message;
			write(mySocket, message, strlen(message));
		}

		memset(message, '\0', sizeof(message));
		recv(mySocket, message, 500, 0);

		if (message[0] == 'N')
		{
			cout << "\n\x1B[31mUser is already registered\033[0m\n\x1B[33mPlease change your UserId or Password.\n\033[0m" << endl;
			continue;
		}

		if(message[0] == 'Y')
			break;
	}
}