#include "utility.h"

vector<string> Utility::split(const string &client_response, string delimiter)
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

string Utility::getCollectionName(string sender, string receiver)
{
    string str = sender + receiver;

    sort(str.begin(), str.end());
    return str;
}