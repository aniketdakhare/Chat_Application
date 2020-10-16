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
    printf("\nStatus: ONLINE\n");
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