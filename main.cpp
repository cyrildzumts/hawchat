#include <iostream>


#include "testserialization.h"
#ifdef _SERVER
#include "server.h"
#else
#include "client.h"
#endif


int main(int argc, char *argv[])
{
    Logger::log("channel size : " + std::to_string(sizeof(Channel)));
#ifdef _SERVER
    Server server;
    server.init();
    server.start();
#else
    Client client;
    client.init();
    client.shell();

    client.logout();

#endif

    std::this_thread::sleep_for(std::chrono::seconds(120));
    //message_test();
    //loginout_test();
    std::cout << "quitting ..." << std::endl;
    return 0;
}
