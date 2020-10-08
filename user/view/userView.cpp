#include "userView.h"

void UserView::displayConnectionStatus(int connectionStatus)
{
    if (connectionStatus < 0)
    {
    	perror("Connection Failed"); 
		exit(EXIT_FAILURE);
    }
    cout << "Connected with Server." << endl;
}