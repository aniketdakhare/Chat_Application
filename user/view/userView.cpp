#include "userView.h"

void UserView::displayConnectionStatus(int connectionStatus)
{
	system("clear");

    if (connectionStatus < 0)
    {
    	perror("Connection Failed"); 
		exit(EXIT_FAILURE);
    }
    cout << "Connected with Server." << endl;
}

string UserView::writeMessage()
{
	string message;
  	cin >> message;
  	return message;
}

void UserView::printMessage(string message)
{
  	cout << "\x1B[33m" << "Server: " << message << "\033[0m" << endl;
}