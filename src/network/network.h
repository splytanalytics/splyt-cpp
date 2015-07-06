#ifndef NETWORK_H_
#define NETWORK_H_

#include <stdio.h>
#include <string>
#include <json/json.h>

#include "splyt.h"

namespace splyt
{
    const std::string kNetworkHost = "https://data.splyt.com";
    const std::string kNetworkSdk = "cpp";
    const std::string kNetworkVersion = "4.0.6";

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
                Log::Info(c.toStyledString());
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
            static void Init(HttpInterface& httpint);
            static NetworkResponse Call(std::string path, Json::Value content, NetworkCallback callback = NULL);
    };
}
#endif  // NETWORK_H_