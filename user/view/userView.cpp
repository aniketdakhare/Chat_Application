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
  	getline(cin, message);
  	return message;
}

void UserView::printMessage(string message)
{
  	cout << "\x1B[33m" << "Server: " << message << "\033[0m" << endl;
}

int UserView::getUserChoice()
{
	int choice;
	cout << "1: Login\n2: Exit\nEnter Your Choice: ";
	cin >> choice;
	return choice;
}

string UserView::getUserName()
{
	string userName;
	cout << "\nEnter UserName: ";
	getline(cin, userName);
	return userName;
}

string UserView::getUserPassword()
{
	string password;
	cout << "\nEnter Password: ";
	getline(cin, password);
	return password;
}

void UserView::displayWarning(string message)
{
	cout << "\x1B[31m" << message << "\033[0m" << endl;
}