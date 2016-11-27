#ifndef LOGGER_H
#define LOGGER_H

#include "common.h"

class Logger
{
public:
    ~Logger();
    static void log(const std::string &text);

};


#endif // LOGGER_H
