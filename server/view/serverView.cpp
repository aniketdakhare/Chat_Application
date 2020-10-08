#include "serverView.h"

void ServerView::displayConnectionStatus(int connectionStatus)
{
    if (connectionStatus < 0)
    {
    	perror("listen"); 
		exit(EXIT_FAILURE);
    }
    cout << "Server started.\nListening...." << endl;
}