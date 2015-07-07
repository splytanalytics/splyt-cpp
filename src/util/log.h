#ifndef SPLYT_LOG_H_
#define SPLYT_LOG_H_

#include <stdio.h>
#include <iostream>
#include <ctime>
#include <string>

namespace splyt
{
    class Log
    {
        private:
            static void LogString(std::string s);
            static std::string CurrentDateTime() {
                time_t     now = time(0);
                struct tm  tstruct;
                char       buf[80];
                tstruct = *localtime(&now);
                // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
                // for more information about date/time format
                strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

                return buf;
            }

        public:
            static void Info(std::string s);
            static void Error(std::string s);
    };
}
#endif  // SPLYT_LOG_H_