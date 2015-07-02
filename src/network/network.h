#ifndef NETWORK_H_
#define NETWORK_H_

#include <stdio.h>
#include <string>
#include "splyt.h"

namespace splyt
{
    const std::string kNetworkHost = "https://data.splyt.com";
    const std::string kNetworkSdk = "cpp";
    const std::string kNetworkVersion = "4.0.6";

    class Network
    {
        private:
            static HttpInterface* httpint;

        public:
            static void Init(HttpInterface& httpint);
            static void Call(std::string path, std::string content);
    };
}
#endif  // NETWORK_H_