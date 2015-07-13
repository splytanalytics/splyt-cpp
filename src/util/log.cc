#include "util/log.h"

namespace splytapi
{
    void Log::LogString(std::string s, bool newline)
    {
        std::string nstring = "[" + Log::CurrentDateTime() + "] " + s;
        std::cout << nstring;
        if (newline) {
            std::cout << "\n";
        }
    }

    void Log::Info(std::string s, bool newline)
    {
        LogString("INFO: " + s, newline);
    }

    void Log::Warn(std::string s, bool newline)
    {
        LogString("WARN: " + s, newline);
    }

    void Log::Error(std::string s, bool newline)
    {
        LogString("ERROR: " + s, newline);
    }
}