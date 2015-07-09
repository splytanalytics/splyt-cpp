#include "util/util.h"

namespace splyt
{
    std::string Util::GetTimestampStr()
    {
        std::stringstream strm;
        strm << (std::time(0) * 1000);
        return strm.str();
    }

    void Util::Sleep(long value)
    {
        #ifdef __unix__
            usleep(value * 1000);
        #elif _WIN32
            Sleep(value);
        #else
            throw std::runtime_error("No sleep function has been defined.");
        #endif
    }
}