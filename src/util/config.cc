#include "util/config.h"

namespace splytapi
{
    //Configuration defaults.
    bool Config::kDebugLog = false;
    std::string Config::kNetworkHost = "https://data.splyt.com";
    std::string Config::kNetworkSdk = "cpp";
    std::string Config::kNetworkVersion = "4.0.6";
    std::string Config::kSsfApp = "isos-personalization";
    std::string Config::kSsfVersion = "4";
    std::string Config::kSsfOutput = "json";
    long Config::kTuningCacheTtl = 10000;
    long Config::kNetworkTimeout = 1500;
}