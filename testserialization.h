#ifndef TESTSERIALIZATION_H
#define TESTSERIALIZATION_H
#include "serialization.h"
struct Message;
struct LogInOut;
void loginout_test();
void message_test();
void print_message(const Message &msg);
void printLogInout(const LogInOut &log);
#endif // TESTSERIALIZATION_H
