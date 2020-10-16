#include "DBOperations.h"

bool DBOperations::checkExists(string userId, string password)
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