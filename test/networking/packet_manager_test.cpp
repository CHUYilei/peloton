//===----------------------------------------------------------------------===//
//
//                         Peloton
//
// packet_manager_test.cpp
//
// Identification: test/wire/packet_manager_test.cpp
//
// Copyright (c) 2016-17, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "common/harness.h"
#include "gtest/gtest.h"
#include "common/logger.h"
#include "wire/libevent_server.h"
//#include <pqxx/pqxx>
//#include "../third_party/libpg_query/src/postgres/include/libpq/libpq.h"
//#include <pthread.h>

#define NUM_THREADS 2

namespace peloton {
namespace test {

//===--------------------------------------------------------------------===//
// Packet Manager Tests
//===--------------------------------------------------------------------===//

class PacketManagerTests: public PelotonTest {
};

static void* LaunchServer(void *) {
    // Launch peloton server
    LOG_INFO("Will launch server!");
    try {
        // Setup
        peloton::PelotonInit::Initialize();
        LOG_INFO("Server initialized");
        // Launch server
        peloton::wire::LibeventServer libeventserver;

        // Teardown
        peloton::PelotonInit::Shutdown();
    } catch (peloton::ConnectionException exception) {
        // Nothing to do here!
    }
    LOG_INFO("Libevent server is launched!");
    return NULL;
}

static void* LaunchClient(void *) {
    LOG_INFO("Will launch client!");
//    try {
//        pqxx::connection C;
//        std::cout << "Connected to " << C.dbname() << std::endl;
//        pqxx::work W(C);
//
//        pqxx::result R = W.exec("SELECT name FROM employee where id=1;");
//
//        LOG_INFO("Found %lu employees",R.size());
//        W.commit();
//    } catch (const std::exception &e) {
//    }

    LOG_INFO("Client is launched!");
    return NULL;
}

TEST_F(PacketManagerTests, WireInitTest) {
//    pthread_t threads[NUM_THREADS];
//    int rc = pthread_create(&threads[0], NULL, LaunchServer, NULL);
//    if (rc) {
//        LOG_INFO("Error:unable to create server thread");
//        exit(-1);
//    }
//
//    rc = pthread_create(&threads[1], NULL, LaunchClient, NULL);
//    if (rc) {
//        LOG_INFO("Error:unable to create client thread");
//        exit(-1);
//    }
//
//    pthread_join(threads[0], NULL);
    std::thread server_td(LaunchServer, this);
    server_td.join();

}

}  // End test namespace
}  // End peloton namespace
