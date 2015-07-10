#include "util/util.h"

namespace splyt
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

        std::stringstream strm;
        strm << t;
        return strm.str();
    }
}