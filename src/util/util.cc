/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#include "util/util.h"

namespace splytapi
{
    long Util::GetTimestamp()
    {
		#pragma warning(push)
		#pragma warning(disable:4244) //Disable warning regarding loss of accuracy, no concern.
        return (std::time(0) * 1000);
		#pragma warning(pop)
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