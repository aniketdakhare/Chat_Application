#include "DBOperations.h"

bool DBOperations::validateUser(string userId, string password)
{
    auto collection = conn["DemoUserDB"]["user"];
    auto cursor = collection.find({});
    bsoncxx::document::element id, pass;

    for(auto&& data : cursor)
    {
        id = data["userId"];
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

    document << "userId" << userId
            << "password" << password;

    collection.insert_one(document.view());
}