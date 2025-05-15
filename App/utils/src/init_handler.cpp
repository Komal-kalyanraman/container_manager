#include "inc/init_handler.hpp"

#include <glog/logging.h>

#include "inc/common.hpp"
#include "inc/redis_database.hpp"

void InitProject() {
    // Initialize logging
    google::InitGoogleLogging(kContainerManagerLogName.c_str());

    // Clear database before starting the server
    IDatabaseHandler& db = RedisDatabaseHandler::GetInstance();
    db.ClearDatabase(); // You need to implement this in RedisDatabaseHandler if not present
}