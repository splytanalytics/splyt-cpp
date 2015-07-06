#include <stdexcept>
#include <stdio.h>

#include "splyt.h"
#include "network/network.h"

namespace splyt
{
    bool initialized = false;
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
        if (splyt::initialized) {
            throw std::runtime_error("Splyt has already been initialized.");
        }

        Log::Info("Splyt init.");

        if(customer_id.empty()) {
            throw std::runtime_error("A customer ID is required.");
        }

        if(user_id.empty() && device_id.empty()) {
            throw std::runtime_error("A user or device ID is required.");
        }

        splyt::customer_id = customer_id;
        splyt::user_id = user_id;
        splyt::device_id = device_id;

        Network::Init(httpint);

        splyt::initialized = true;
    }

    Json::Value BeginTransaction(std::string user_id, std::string device_id, std::string category, std::string timeout_mode, int timeout, std::string transaction_id, Json::Value properties)
    {
        Json::Value json;

        std::string ts = GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(device_id);
        json.append(category);
        json.append(timeout_mode);
        json.append(timeout);
        json.append(transaction_id);
        json.append(properties);

        NetworkResponse resp = Network::Call("datacollector_beginTransaction", json);
        if (!resp.IsSuccessful()) {
            throw std::runtime_error("Splyt Error: " + resp.GetErrorMessage());
        }

        return resp.GetContent()["datacollector_beginTransaction"]["data"];
    }

    Json::Value EndTransaction(std::string user_id, std::string device_id, std::string category, std::string result, std::string transaction_id, Json::Value properties)
    {
        Json::Value json;

        std::string ts = GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(device_id);
        json.append(category);
        json.append(result);
        json.append(transaction_id);
        json.append(properties);

        NetworkResponse resp = Network::Call("datacollector_endTransaction", json);
        if (!resp.IsSuccessful()) {
            throw std::runtime_error("Splyt Error: " + resp.GetErrorMessage());
        }

        return resp.GetContent()["datacollector_endTransaction"]["data"];
    }
}
