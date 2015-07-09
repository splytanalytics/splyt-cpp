#include "util/util.h"

namespace splyt
{
    std::string Util::GetTimestampStr()
    {
        std::stringstream strm;
        strm << (std::time(0) * 1000);
        return strm.str();
    }
}