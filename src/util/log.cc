#include "util/log.h"

namespace splyt
{
    void Log::LogString(std::string s)
    {
        std::string nstring = "[" + Log::CurrentDateTime() + "] " + s;
        std::cout << nstring + "\n";
    }

    void Log::Info(std::string s)
    {
        LogString("INFO: " + s);
    }

    void Log::Error(std::string s)
    {
        LogString("ERROR: " + s);
    }
}