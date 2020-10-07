#include "../model/server.h"
#include "../view/serverView.h"

int main()
{
    Server server;
    ServerView serverView;
    serverView.displayConnectionStatus(server.getConnectionStatus());
    server.getConnectedUsersSocket();
}