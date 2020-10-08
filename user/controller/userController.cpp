#include "../model/user.h"
#include "../view/userView.h"

int main()
{
    User user;
    UserView userView;
    userView.displayConnectionStatus(user.getConnectionStatus());
}