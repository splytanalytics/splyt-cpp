#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include "splyt.h"

namespace splyt
{
    class Log
    {
        private:
            static void LogString(std::string s);

        public:
            static void Info(std::string s);
            static void Error(std::string s);
    };
}
#endif  // LOG_H_