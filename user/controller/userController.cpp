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

    void startUser()
    {
        userView.displayConnectionStatus(user.getConnectionStatus());
        int userSocket = user.getUserSocket();

        thread send_thread (&UserController::sendMessage, this, userSocket);
	    thread recv_thread (&UserController::readMessage, this, userSocket);

	    send_thread.join();
	    recv_thread.join();
    }

    public:
    void loginUser()
    {
        int choice = userView.getUserChoice();

        switch(choice)
        {
            case 1:
                {
                    bool flag = true;
                    while(flag)
                    {
                        string userName = userView.getUserName();
                        string password = userView.getUserPassword();
                        bool checkCredentials = user.ckeckUserCredentials(userName, password);
                        if(checkCredentials)
                            flag = false;
                        else
                            userView.displayWarning("Invalid Username or Password");
                    }
                    startUser();
                }
                break;
            case 2:
                exit(0);
        }
    }
};

int main()
{
    UserController controller;
    controller.loginUser();
}