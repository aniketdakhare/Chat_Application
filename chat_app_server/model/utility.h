#pragma once
#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct Utility
{
    vector<string> split(const string &client_response, string delimiter)
    {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        string token;
        vector<string> result;

        while ((pos_end = client_response.find(delimiter, pos_start)) != string::npos)
        {
            token = client_response.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            result.push_back(token);
        }
        result.push_back(client_response.substr(pos_start));
        return result;
    }
};