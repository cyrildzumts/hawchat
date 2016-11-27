#ifndef MESSAGE_H
#define MESSAGE_H
#include "common.h"

// Type CONSTANT
constexpr int LOGINOUT = 1;
constexpr int CONTROLINFO = 2;
constexpr int MSG = 3;

// FLAGS
constexpr int SYN = 1;
constexpr int ACK = 2;
constexpr int FIN = 4;
constexpr int DUP = 8;
constexpr int GET = 16;

constexpr int STR_LEN = 16;


typedef uint8_t Byte;

/**
 * @brief The header struct
 * this the common header used to pass message
 * Type : 1 LogInOut
 *        2 ControlInfo
 *        3 Message
 *
 * Flags : 1 SYN
 *         2 ACK
 *         4 FIN
 *         8 DUP
 *         16 GET
 */
struct Header
{
    Byte length;
    Byte version;
    Byte flags;
    Byte type;
};
union flat_header
{
    int value;
    Header header;
};

struct LogInOut
{
    // length byte = max 16 byte
    Header header;
    char username[STR_LEN];
};
struct Entry
{
    Byte hops;
    Byte port;
    char host[STR_LEN];
    char username[STR_LEN];
};

struct ControlInfo
{
    Header header;
    Entry *entries;
};

struct Message
{
    Header header;      // 4 Byte
    char sender[STR_LEN];    // 16 Byte
    char receiver[STR_LEN];  // 16 Byte
    char* data;         // X Byte
};

Message build_message(const std::string &sender,
                      const std::string &receiver,
                      const char *data, int len);

LogInOut build_loginout(const std::string &username);

/*
Message build_message(const std::string &sender,
                      const std::string &receiver,
                      const std::string &data);
                      */


void messageTest(const void *data);


#endif // MESSAGE_H
