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

    Json::Value HandleResponse(std::string type, NetworkResponse resp, NetworkCallback callback)
    {
        if(callback != NULL) {
            return Json::Value::null;
        }

        if (!resp.IsSuccessful()) {
            throw std::runtime_error("Splyt Error: " + resp.GetErrorMessage());
        }

        return resp.GetContent()[type]["data"];
    }

    Json::Value NewUser(std::string user_id, NetworkCallback callback)
    {
        Json::Value json;

        std::string ts = GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(Json::Value::null);

        NetworkResponse resp = Network::Call("datacollector_newUser", json, callback);
        return HandleResponse("datacollector_newUser", resp, callback);
    }

    Json::Value NewDevice(std::string device_id, NetworkCallback callback)
    {
        Json::Value json;

        std::string ts = GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(Json::Value::null);
        json.append(device_id);

        NetworkResponse resp = Network::Call("datacollector_newDevice", json, callback);
        return HandleResponse("datacollector_newDevice", resp, callback);
    }

    Json::Value NewUserChecked(std::string user_id, NetworkCallback callback)
    {
        Json::Value json;

        std::string ts = GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(Json::Value::null);

        NetworkResponse resp = Network::Call("datacollector_newUserChecked", json, callback);
        return HandleResponse("datacollector_newUserChecked", resp, callback);
    }

    Json::Value NewDeviceChecked(std::string device_id, NetworkCallback callback)
    {
        Json::Value json;

        std::string ts = GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(Json::Value::null);
        json.append(device_id);

        NetworkResponse resp = Network::Call("datacollector_newDeviceChecked", json, callback);
        return HandleResponse("datacollector_newDeviceChecked", resp, callback);
    }

    Json::Value BeginTransaction(std::string user_id, std::string device_id, std::string category, int timeout, std::string transaction_id, Json::Value properties, NetworkCallback callback)
    {
        Json::Value json;

        std::string ts = GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(device_id);
        json.append(category);
        json.append("TXN");
        json.append(timeout);
        json.append(transaction_id);
        json.append(properties);

        NetworkResponse resp = Network::Call("datacollector_beginTransaction", json, callback);
        return HandleResponse("datacollector_beginTransaction", resp, callback);
    }

    Json::Value UpdateTransaction(std::string user_id, std::string device_id, std::string category, int progress, std::string transaction_id, Json::Value properties, NetworkCallback callback)
    {
        Json::Value json;

        std::string ts = GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(device_id);
        json.append(category);
        json.append(progress);
        json.append(transaction_id);
        json.append(properties);

        NetworkResponse resp = Network::Call("datacollector_updateTransaction", json, callback);
        return HandleResponse("datacollector_updateTransaction", resp, callback);
    }

    Json::Value EndTransaction(std::string user_id, std::string device_id, std::string category, std::string result, std::string transaction_id, Json::Value properties, NetworkCallback callback)
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

        NetworkResponse resp = Network::Call("datacollector_endTransaction", json, callback);
        return HandleResponse("datacollector_endTransaction", resp, callback);
    }
}
