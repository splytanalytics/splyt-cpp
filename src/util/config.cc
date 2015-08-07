/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#include "util/config.h"

namespace splytapi
{
    //Configuration defaults.
    bool Config::kDebugLog = false;
    std::string Config::kNetworkHost = "https://data.splyt.com";
	bool Config::kNetworkEnableVerifyPeer = true;
    std::string Config::kSsfApp = "isos-personalization";
    std::string Config::kSsfVersion = "4";
    std::string Config::kSsfOutput = "json";
    long Config::kTuningCacheTtl = 900000;
    long Config::kNetworkTimeout = 5;
}