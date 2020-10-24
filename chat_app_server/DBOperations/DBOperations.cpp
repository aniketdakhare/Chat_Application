#include "DBOperations.h"

bool DBOperations::validateUser(string userId, string password)
{
    mongocxx::collection collection = conn[db]["user"];
    mongocxx::cursor cursor = collection.find({});
    bsoncxx::document::element id, pass;

    for (auto &&data : cursor)
    {
        id = data["user_id"];
        pass = data["password"];
        if (id.get_utf8().value == userId && pass.get_utf8().value == password)
        {
            return true;
        }
    }

    return false;
}

void DBOperations::registerUser(string userId, string password)
{
    mongocxx::collection collection = conn[db]["user"];

    auto builder = bsoncxx::builder::stream::document{};

    bsoncxx::document::value data = builder
                                    << "user_id" << userId
                                    << "password" << password
                                    << "online_status" << false
                                    << "socket_number" << 1 << finalize;

    collection.insert_one(data.view());
}

bool DBOperations::checkUserExists(string userId)
{
    mongocxx::collection collection = conn[db]["user"];
    mongocxx::cursor cursor = collection.find({});

    for (auto &&data : cursor)
    {
        if (data["user_id"].get_utf8().value == userId)
        {
            return true;
        }
    }

    return false;
}

void DBOperations::updateOnlineStatus(ClientInfo client)
{
    mongocxx::collection collection = conn[db]["user"];

    collection.update_one(
        make_document(kvp("user_id", client.userId)),
        make_document(kvp("$set", make_document(
                                      kvp("online_status", client.loginStatus),
                                      kvp("socket_number", client.mySocket)))));
}

vector<ClientInfo> DBOperations::getRegisteredClientsList()
{
    vector<ClientInfo> registeredClientsList;

    mongocxx::collection collection = conn[db]["user"];

    mongocxx::cursor result = collection.find({});

    for (auto client : result)
    {
        ClientInfo cl;
        cl.userId = client["user_id"].get_utf8().value;
        cl.loginStatus = client["online_status"].get_bool().value;
        cl.mySocket = client["socket_number"].get_int32();
        cl.password = client["password"].get_utf8().value;

        registeredClientsList.push_back(cl);
    }
    return registeredClientsList;
}

void DBOperations::storeClientMessages(string sender, string receiver, string message)
{
    string collectionName = util.getCollectionName(sender, receiver);
    mongocxx::collection collection = conn[db][collectionName];

    auto time = chrono::system_clock::to_time_t(chrono::system_clock::now());

    auto builder = bsoncxx::builder::stream::document{};

    bsoncxx::document::value data = builder
                                    << "sender" << sender
                                    << "receiver" << receiver
                                    << "message" << message
                                    << "time" << time << finalize;

    collection.insert_one(data.view());
}

vector<pair<string, string>> DBOperations::getClientMessages(string sender, string receiver)
{
    vector<pair<string, string>> messages;
    bsoncxx::document::element msg;
    string collectionName = util.getCollectionName(sender, receiver);
    mongocxx::collection collection = conn[db][collectionName];

    mongocxx::cursor result = collection.find({});

    for (bsoncxx::document::view data : result)
    {
        pair<string, string> message;
        message.first = data["sender"].get_utf8().value;
        message.second = data["message"].get_utf8().value;
        if (data["message"].get_utf8().value.size() > 0)
        {
            messages.push_back(message);
        }
    }
    return messages;
}