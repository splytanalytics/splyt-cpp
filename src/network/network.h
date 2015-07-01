#ifndef NETWORK_H_
#define NETWORK_H_

#include <stdio.h>
#include <string>
#include "splyt.h"

namespace splyt
{
    const std::string network_host = "https://data.splyt.com";
    const std::string network_sdkname = "cpp";
    const std::string network_sdkversion = "4.0";

    class Network
    {
        private:
            static HttpInterface* httpint;

        public:
            static void Init(HttpInterface& httpint);
            static void Call(std::string path);
    };
}
#endif  // NETWORK_H_