#include "serverView.h"

void ServerView::displayConnectionStatus(int connectionStatus)
{
	system("clear");

    if (connectionStatus < 0)
    {
    	perror("listen"); 
		exit(EXIT_FAILURE);
    }
    cout << "Server started.\nListening...." << endl;
}

string ServerView::writeMessage()
{
	string message;
  	cin >> message;
  	return message;
}

void ServerView::printMessage(string message)
{
  	cout << "\x1B[33m" << "User: " << message << "\033[0m" << endl;
}