#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>
#include <functional>

#define SERVER_PORT "50000"
#define CLIENT_PORT "60000"
#define BACKLOG 50
#define SOCKET_ERROR -1

#define LEN 30

enum SERVER_SERVICE
{
    LOGIN,
    LOGOUT,
    SENDMSG,
    GETUSERS,
    HEARTBEAT,
};

#endif // COMMON_H
