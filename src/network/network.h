/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#ifndef SPLYT_NETWORK_H_
#define SPLYT_NETWORK_H_

#include <string>
#include <stdexcept>

#include "splyt.h"

namespace splytapi
{
    class Network
    {
        private:
            Splyt* s;
            HttpInterface* httpint;

        public:
            Network(Splyt* sp);
            ~Network();

            /** Initialize the networking class used to make calls to the Splyt API and
                send the initial application_init request.

                @param httpint - HttpInterface used for HTTP requests.

                @return Json::Value
            */
            Json::Value Init(HttpInterface* http);

            /** Make a call to the Splyt API.

                @param std::string sub_path - The path for the API call.
                @param Json::Value content - JSON content sent to Splyt.
                @param std::string context - The context of the call.

                @return SplytResponse - Object that contains error data or the JSON content.
            */
            SplytResponse Call(std::string path, Json::Value content, std::string context = "defaultContext", NetworkCallback callback = NULL);

            static SplytResponse ParseResponse(std::string str_response);
            static std::string InterpretError(int code);
    };
}
#endif  // SPLYT_NETWORK_H_