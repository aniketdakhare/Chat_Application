#pragma once

#include <iostream>

using namespace std;

struct ServerView
{
    void displayConnectionStatus(int);
    string writeMessage();
    void printMessage(string);
};