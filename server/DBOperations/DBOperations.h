#pragma once
#include <iostream>
#include <string>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

using namespace std;

class DBOperations
{
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{}};

    bsoncxx::builder::stream::document document{};

    public:
        bool checkExists(string, string);
};