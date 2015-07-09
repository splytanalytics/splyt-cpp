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

    void Init(std::string customer_id, std::string user_id, std::string device_id) {
        splyt::CurlHttpInterface* httpint = new splyt::CurlHttpInterface();
        splyt::Init(customer_id, user_id, device_id, httpint);
    }

    void Init(std::string customer_id, std::string user_id, std::string device_id, HttpInterface* httpint) {
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

    SplytResponse HandleResponse(std::string type, SplytResponse resp, NetworkCallback callback)
    {
        if(callback != NULL) {
            return resp;
        }

        if (!resp.IsSuccessful()) {
            //throw std::runtime_error("Splyt Error: " + resp.GetErrorMessage());
            Log::Error("Splyt Error Response: " + resp.GetContent().toStyledString());
        }

        //return resp.GetContent()[type]["data"];
        return resp;
    }

    SplytResponse NewUser(std::string user_id, NetworkCallback callback)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(Json::Value::null);

        SplytResponse resp = Network::Call("datacollector_newUser", json, callback);
        return HandleResponse("datacollector_newUser", resp, callback);
    }

    SplytResponse NewDevice(std::string device_id, NetworkCallback callback)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(Json::Value::null);
        json.append(device_id);

        SplytResponse resp = Network::Call("datacollector_newDevice", json, callback);
        return HandleResponse("datacollector_newDevice", resp, callback);
    }

    SplytResponse NewUserChecked(std::string user_id, NetworkCallback callback)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(Json::Value::null);

        SplytResponse resp = Network::Call("datacollector_newUserChecked", json, callback);
        return HandleResponse("datacollector_newUserChecked", resp, callback);
    }

    SplytResponse NewDeviceChecked(std::string device_id, NetworkCallback callback)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(Json::Value::null);
        json.append(device_id);

        SplytResponse resp = Network::Call("datacollector_newDeviceChecked", json, callback);
        return HandleResponse("datacollector_newDeviceChecked", resp, callback);
    }

    SplytResponse BeginTransaction(std::string user_id, std::string device_id, std::string category, int timeout, std::string transaction_id, Json::Value properties, NetworkCallback callback)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(device_id);
        json.append(category);
        json.append("TXN");
        json.append(timeout);
        json.append(transaction_id);
        json.append(properties);

        SplytResponse resp = Network::Call("datacollector_beginTransaction", json, callback);
        return HandleResponse("datacollector_beginTransaction", resp, callback);
    }

    SplytResponse UpdateTransaction(std::string user_id, std::string device_id, std::string category, int progress, std::string transaction_id, Json::Value properties, NetworkCallback callback)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(device_id);
        json.append(category);
        json.append(progress);
        json.append(transaction_id);
        json.append(properties);

        SplytResponse resp = Network::Call("datacollector_updateTransaction", json, callback);
        return HandleResponse("datacollector_updateTransaction", resp, callback);
    }

    SplytResponse EndTransaction(std::string user_id, std::string device_id, std::string category, std::string result, std::string transaction_id, Json::Value properties, NetworkCallback callback)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(device_id);
        json.append(category);
        json.append(result);
        json.append(transaction_id);
        json.append(properties);

        SplytResponse resp = Network::Call("datacollector_endTransaction", json, callback);
        return HandleResponse("datacollector_endTransaction", resp, callback);
    }
}
