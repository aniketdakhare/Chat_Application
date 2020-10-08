#include "../model/user.h"
#include "../view/userView.h"
#include <thread>

class UserController
{
    User user;
    UserView userView;
    
    void sendMessage(int socket)
    {
        while (true)
        {
            user.sendMessage(userView.writeMessage(), socket);   
        }        
    }

    void readMessage(int socket)
    {
        while (true)
        {
            userView.printMessage(user.receiveMessage(socket));
        }        
    }


    public:
    
    void startUser()
    {
        userView.displayConnectionStatus(user.getConnectionStatus());
        int userSocket = user.getUserSocket();

        thread send_thread (&UserController::sendMessage, this, userSocket);
	    thread recv_thread (&UserController::readMessage, this, userSocket);

	    send_thread.join();
	    recv_thread.join();
    }
};

int main()
{
    UserController controller;
    controller.startUser();
}