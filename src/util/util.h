/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#ifndef SPLYT_UTIL_H_
#define SPLYT_UTIL_H_

#include <sstream>
#include <ctime>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

namespace splytapi
{
    class Util
    {
        public:
            static long GetTimestamp();
            static std::string GetTimestampStr(long t = 0);
            template < typename T >
            static std::string ToString( const T& n );

            static inline std::string &LTrim(std::string &s) {
                s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
                return s;
            }

            static inline std::string &RTrim(std::string &s) {
                s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
                return s;
            }

            static inline std::string &Trim(std::string &s) {
                return LTrim(RTrim(s));
            }
    };
}
#endif  // SPLYT_UTIL_H_