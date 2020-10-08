#include "../model/server.h"
#include "../view/serverView.h"
#include <thread>

class ServerController
{
    Server server;
    ServerView serverView;
    
    void sendMessage(int socket)
    {
        while (true)
        {
            server.sendMessage(serverView.writeMessage(), socket);   
        }        
    }

    void readMessage(int socket)
    {
        while (true)
        {
            serverView.printMessage(server.receiveMessage(socket));
        }        
    }

    public:
    
    void startServer()
    {
        serverView.displayConnectionStatus(server.getConnectionStatus());
        int userSocket = server.getConnectedUsersSocket();

        thread send_thread (&ServerController::sendMessage, this, userSocket);
	    thread recv_thread (&ServerController::readMessage, this, userSocket);

	    send_thread.join();
	    recv_thread.join();
    }
};

int main()
{
    ServerController controller;
    controller.startServer();
}