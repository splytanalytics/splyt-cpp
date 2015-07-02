#include <stdio.h>

#include "splyt.h"
#include "util/log.h"
#include "network/network.h"

namespace splyt
{
    std::string customer_id = "";
    std::string user_id = "";
    std::string device_id = "";

    /** Initialize the Splyt SDK.
        This function must be called first to use any functions provided in the SDK.

        NOTE: user_id and device_id are optional, but you must at least pass one or the other.
        @param httpint - HttpInterface used for HTTP requests.
        @param customer_id - Customer ID provided by Splyt.
        @param user_id
        @param device_id
    */
    void Init(HttpInterface& httpint, std::string customer_id, std::string user_id, std::string device_id) {
        Log::Info("Splyt init.");

        splyt::customer_id = customer_id;
        splyt::user_id = user_id;
        splyt::device_id = device_id;

        Network::Init(httpint);
    }
}
