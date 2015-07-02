#include "network/network.h"
#include "util/log.h"

namespace splyt
{
    HttpInterface* Network::httpint;

    /** Initialize the networking class used to make calls to the Splyt API.

        @param httpint - HttpInterface used for HTTP requests.
    */
    void Network::Init(HttpInterface& httpint)
    {
        Network::httpint = &httpint;
        Log::Info("Network init.");
    }

    /** Make a call to the Splyt API.

        @param sub_path - The path for the API call.
        @param content - Content sent to Splyt.
    */
    void Network::Call(std::string sub_path, std::string content, NetworkCallback callback)
    {
        if(!Network::httpint) {
            Log::Error("No HTTP implementation available. Did you call splyt::Init()?");
            return;
        }

        std::string path = "/isos-personalization/ws/interface/" + sub_path;

        std::string query = "?";
        query.append(SPLYT_QPARM_WSVERSION);
        query.append("&ssf_cust_id=");
        query.append(splyt::customer_id);
        query.append("&");
        query.append(SPLYT_QPARM_OUTPUT);
        query.append("&ssf_sdk=");
        query.append(kNetworkSdk);
        query.append("&ssf_sdk_version=");
        query.append(kNetworkVersion);
        query.append("&ssf_sdk_contextname=testContext");

        const char* headers[] = {
            "ssf-use-positional-post-params: true",
            "ssf-contents-not-url-encoded: true"
        };

        Network::httpint->Post(kNetworkHost, path + query, content);

        callback(1);
    }
}