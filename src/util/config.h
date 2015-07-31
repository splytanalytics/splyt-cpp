/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
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

            //SSF app. Ex: isos-personalization
            static std::string kSsfApp;

            //SSF Version. Ex: 4
            static std::string kSsfVersion;

            //SSF Output. Ex: json
            static std::string kSsfOutput;

            //Tuning variable cache time to live, measured in milliseconds. Ex. 900000 - 15 Minutes
            static long kTuningCacheTtl;

            //Network call timeout, measured in seconds. Ex. 5
            static long kNetworkTimeout;
    };
}
#endif  // SPLYT_CONFIG_H_