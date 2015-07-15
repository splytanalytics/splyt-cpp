#ifndef SPLYT_UTIL_H_
#define SPLYT_UTIL_H_

#include <sstream>
#include <ctime>
#include <string>
#include <stdexcept>

namespace splytapi
{
    class Util
    {
        public:
            static long GetTimestamp();
            static std::string GetTimestampStr(long t = 0);
            template < typename T >
            static std::string ToString( const T& n );
    };
}
#endif  // SPLYT_UTIL_H_