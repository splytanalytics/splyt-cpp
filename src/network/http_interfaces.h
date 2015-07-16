/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#ifndef SPLYT_HTTP_INTERFACES_H_
#define SPLYT_HTTP_INTERFACES_H_

#include <curl/curl.h>
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

    class CurlHttpInterface : public splytapi::HttpInterface
    {
        private:
            static size_t write_to_string(void *ptr, size_t size, size_t count, void *stream);

        public:
            CurlHttpInterface();
            ~CurlHttpInterface(){}
            virtual std::string Post(std::string url, std::string path, std::string headers[], int header_count, std::string content, long timeout);
    };
}
#endif  // SPLYT_HTTP_INTERFACES_H_