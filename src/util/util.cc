#include "util/util.h"

namespace splytapi
{
    long Util::GetTimestamp()
    {
        return (std::time(0) * 1000);
    }

    std::string Util::GetTimestampStr(long t)
    {
        if (t == 0) {
            t = Util::GetTimestamp();
        }

        return Util::ToString(t);
    }

    template < typename T > std::string Util::ToString( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}