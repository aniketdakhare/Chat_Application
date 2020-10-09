#pragma once

#include <iostream>

using namespace std;

struct UserView
{
    void displayConnectionStatus(int);
    string writeMessage();
    void printMessage(string);
    int getUserChoice();
    string getUserName();
    string getUserPassword();
    void displayWarning(string);
};