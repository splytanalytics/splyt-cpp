#ifndef SPLYT_NETWORK_H_
#define SPLYT_NETWORK_H_

#include <stdio.h>
#include <string>
#include <json/json.h>

#include "splyt.h"

namespace splyt
{
    class NetworkResponse
    {
        private:
            bool success;
            std::string error_message;
            Json::Value content;

        public:
            NetworkResponse(bool s) {
                success = s;
            }

            void SetErrorMessage(std::string err) {
                error_message = err;
            }

            void SetContent(Json::Value c) {
                content = c;
            }

            bool IsSuccessful() {
                return success;
            }

            std::string GetErrorMessage() {
                return error_message;
            }

            Json::Value GetContent() {
                return content;
            }
    };

    typedef void (*NetworkCallback)(NetworkResponse);

    class Network
    {
        private:
            static HttpInterface* httpint;
            static std::string InterpretError(int code);

        public:
            /** Initialize the networking class used to make calls to the Splyt API and
                send the initial application_init request.

                @param httpint - HttpInterface used for HTTP requests.
            */
            static void Init(HttpInterface& httpint);

            /** Make a call to the Splyt API.

                @param std::string sub_path - The path for the API call.
                @param Json::Value content - JSON content sent to Splyt.
                @param NetworkCallback callback - Optional callback for async calls.

                @return NetworkResponse - Object that contains error data or the JSON content.
            */
            static NetworkResponse Call(std::string path, Json::Value content, NetworkCallback callback = NULL);
    };
}
#endif  // SPLYT_NETWORK_H_