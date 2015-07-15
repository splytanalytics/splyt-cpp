#ifndef SPLYT_CONFIG_H_
#define SPLYT_CONFIG_H_

#include <string>

namespace splytapi
{
    class Config {
        public:
            //Debug logging.
            static bool kDebugLog;

            //Splyt API host. -- Only enterprise customers need this. Ex: https://data.splyt.com
            static std::string kNetworkHost;

            //Splyt SDK name. Ex: cpp
            static std::string kNetworkSdk;

            //Splyt SDK version. Ex: 4.0.6
            static std::string kNetworkVersion;

            //SSF app. Ex:
            static std::string kSsfApp;

            //SSF Version. Ex: 4
            static std::string kSsfVersion;

            //SSF Output. Ex: json
            static std::string kSsfOutput;

            //Tuning variable cache time to live. Ex. 3600 - 1 Hour
            static long kTuningCacheTtl;

            //Network call timeout, measured in milliseconds. Ex. 1500
            static long kNetworkTimeout;
    };
}
#endif  // SPLYT_CONFIG_H_