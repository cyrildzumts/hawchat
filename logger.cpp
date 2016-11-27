#include "logger.h"

static std::mutex mux;

Logger::~Logger()
{

}

void Logger::log(const std::string &text)
{
    std::lock_guard<std::mutex>lk(mux);
    std::cout << text << std::flush << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
