#ifndef SPLYT_UTIL_H_
#define SPLYT_UTIL_H_

#ifdef __unix__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

#include <sstream>
#include <ctime>
#include <string>
#include <stdexcept>

namespace splyt
{
    class Util
    {
        public:
            static std::string GetTimestampStr();
            static void Sleep(long value);
    };
}
#endif  // SPLYT_UTIL_H_