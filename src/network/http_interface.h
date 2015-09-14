/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#ifndef SPLYT_HTTP_INTERFACE_H_
#define SPLYT_HTTP_INTERFACE_H_

#include <string>
#include <stdexcept>
#include <jsoncpp/json/json.h>

#include "util/log.h"
#include "network/splyt_response.h"

namespace splytapi
{
    class Splyt;

    class HttpInterface
    {
        protected:
            HttpInterface(){}

        public:
            virtual ~HttpInterface(){}
            //Note: The NetworkCallback callback argument is currently only used for the Unreal Engine, as it does not use the thread manager.
            virtual std::string Post(std::string url, std::string path, std::string headers[], int header_count, std::string content, long timeout, NetworkCallback callback = NULL) = 0;
    };
}
#endif  // SPLYT_HTTP_INTERFACE_H_