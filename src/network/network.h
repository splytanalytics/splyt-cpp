#ifndef SPLYT_NETWORK_H_
#define SPLYT_NETWORK_H_

#include <stdio.h>
#include <string>
#include <stdexcept>
#include <json/json.h>

#include "splyt.h"
#include "network/network_defs.h"

namespace splyt
{
    class Network
    {
        private:
            static HttpInterface* httpint;
            static std::string InterpretError(int code);
            static SplytResponse PerformCall(std::string path, Json::Value content, NetworkCallback callback = NULL);
            static SplytResponse ParseResponse(std::string str_response);

        public:
            /** Initialize the networking class used to make calls to the Splyt API and
                send the initial application_init request.

                @param httpint - HttpInterface used for HTTP requests.
            */
            static void Init(HttpInterface* httpint);

            /** Make a call to the Splyt API.

                @param std::string sub_path - The path for the API call.
                @param Json::Value content - JSON content sent to Splyt.
                @param NetworkCallback callback - Optional callback for async calls.

                @return SplytResponse - Object that contains error data or the JSON content.
            */
            static SplytResponse Call(std::string path, Json::Value content, NetworkCallback callback = NULL);
    };
}
#endif  // SPLYT_NETWORK_H_