#include "DBOperations.h"

bool DBOperations::validateUser(string userId, string password)
{
    auto collection = conn["DemoUserDB"]["user"];
    auto cursor = collection.find({});
    bsoncxx::document::element id, pass;

    for(auto&& data : cursor)
    {
        id = data["user_id"];
        pass = data["password"];
        if(id.get_utf8().value == userId && pass.get_utf8().value == password)
        {
            return true;
        }
    }
    
    return false;
}

void DBOperations::registerUser(string userId, string password)
{
    auto collection = conn["DemoUserDB"]["user"];

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
    mongocxx::collection collection = conn["DemoUserDB"]["user"];
    mongocxx::cursor cursor = collection.find({});

    for(auto&& data : cursor)
    {
        if(data["user_id"].get_utf8().value == userId)
        {
            return true;
        }
    }
    
    return false;
}

void DBOperations::updateOnlineStatus(ClientInfo client)
{
    mongocxx::collection collection = conn["DemoUserDB"]["user"];

    collection.update_one(
        make_document(kvp("user_id", client.userId)),
        make_document( kvp("$set", make_document(
                                    kvp("online_status", client.loginStatus),
                                    kvp("socket_number", client.mySocket)
                                    ))));
}

vector<ClientInfo> DBOperations::getRegisteredClientsList()
{
    vector<ClientInfo> registeredClientsList;

    mongocxx::collection collection = conn["DemoUserDB"]["user"];

    mongocxx::cursor result = collection.find({});

    for(auto client : result)
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