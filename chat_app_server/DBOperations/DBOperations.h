#pragma once
#include <iostream>
#include <string>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <vector>
#include "../model/user.h"
#include "../model/utility/utility.h"

using namespace std;

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_document;

class DBOperations
{
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{}};
    bsoncxx::builder::stream::document document{};
    bsoncxx::builder::basic::document basicDocument{};

    Utility util;

    string db = "ChatApplication";

public:
    void registerUser(string, string);
    bool validateUser(string, string);
    bool checkUserExists(string);
    void updateOnlineStatus(ClientInfo);
    vector<ClientInfo> getRegisteredClientsList();
    void storeClientMessages(string, string, string);
    vector<pair<string, string>> getClientMessages(string, string);
};