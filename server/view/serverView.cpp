#include "serverView.h"

void ServerView::displayConnectionStatus(int connectionStatus)
{
    if (connectionStatus < 0)
    {
    	perror("listen"); 
		exit(EXIT_FAILURE);
    }
    std::cout << "Server started.\nListening...." << std::endl;
}