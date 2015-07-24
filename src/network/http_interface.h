/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#ifndef SPLYT_HTTP_INTERFACE_H_
#define SPLYT_HTTP_INTERFACE_H_

#include <string>
#include <stdexcept>

#include "util/log.h"

namespace splytapi
{
    class Splyt;

    class HttpInterface
    {
        protected:
            HttpInterface(){}

        public:
            virtual ~HttpInterface(){}
            virtual std::string Post(std::string url, std::string path, std::string headers[], int header_count, std::string content, long timeout) = 0;
    };
}
#endif  // SPLYT_HTTP_INTERFACE_H_