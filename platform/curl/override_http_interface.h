/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#ifndef SPLYT_CURL_HTTP_INTERFACE_H_
#define SPLYT_CURL_HTTP_INTERFACE_H_

#include <curl/curl.h>
#include <string>
#include <stdexcept>

#include "network/http_interface.h"

namespace splytapi
{
    class Splyt;

    class OverrideHttpInterface : public splytapi::HttpInterface
    {
        private:
            static size_t write_to_string(void *ptr, size_t size, size_t count, void *stream);

        public:
            OverrideHttpInterface();
            ~OverrideHttpInterface(){}
            virtual std::string Post(std::string url, std::string path, std::string headers[], int header_count, std::string content, long timeout);
    };
}
#endif  // SPLYT_CURL_HTTP_INTERFACE_H_