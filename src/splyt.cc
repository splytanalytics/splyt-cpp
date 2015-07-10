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

    void Init(std::string customer_id, std::string user_id, std::string device_id, std::string context) {
        CurlHttpInterface* httpint = new CurlHttpInterface();
        Init(customer_id, user_id, device_id, context, httpint);
    }

    void Init(std::string customer_id, std::string user_id, std::string device_id, std::string context, HttpInterface* httpint) {
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

    void AppendUD(Json::Value* json, std::string user_id, std::string device_id)
    {
        if (user_id.empty()) {
            if (splyt::user_id.empty()) {
                json->append(Json::Value::null);
            } else {
                json->append(splyt::user_id);
            }
        } else {
            json->append(user_id);
        }

        if (device_id.empty()) {
            if (splyt::device_id.empty()) {
                json->append(Json::Value::null);
            } else {
                json->append(splyt::device_id);
            }
        } else {
            json->append(device_id);
        }
    }

    SplytResponse HandleResponse(std::string type, SplytResponse resp)
    {
        if (!resp.IsSuccessful()) {
            throw std::runtime_error("Splyt Error: " + resp.GetErrorMessage());
        }

        return resp;
    }

    SplytResponse NewUser(std::string user_id, std::string context)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(Json::Value::null);

        SplytResponse resp = Network::Call("datacollector_newUser", json, context);
        return HandleResponse("datacollector_newUser", resp);
    }

    SplytResponse NewDevice(std::string device_id, std::string context)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(Json::Value::null);
        json.append(device_id);

        SplytResponse resp = Network::Call("datacollector_newDevice", json, context);
        return HandleResponse("datacollector_newDevice", resp);
    }

    SplytResponse NewUserChecked(std::string user_id, std::string context)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(Json::Value::null);

        SplytResponse resp = Network::Call("datacollector_newUserChecked", json, context);
        return HandleResponse("datacollector_newUserChecked", resp);
    }

    SplytResponse NewDeviceChecked(std::string device_id, std::string context)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(Json::Value::null);
        json.append(device_id);

        SplytResponse resp = Network::Call("datacollector_newDeviceChecked", json, context);
        return HandleResponse("datacollector_newDeviceChecked", resp);
    }

    SplytResponse BeginTransaction(std::string category, int timeout, std::string transaction_id, std::string context, std::string user_id, std::string device_id, Json::Value properties)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        splyt::AppendUD(&json, user_id, device_id);
        json.append(category);
        json.append("TXN");
        json.append(timeout);
        json.append(transaction_id);
        json.append(properties);

        SplytResponse resp = Network::Call("datacollector_beginTransaction", json, context);
        return HandleResponse("datacollector_beginTransaction", resp);
    }

    SplytResponse UpdateTransaction(std::string category, int progress, std::string transaction_id, std::string context, std::string user_id, std::string device_id, Json::Value properties)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        splyt::AppendUD(&json, user_id, device_id);
        json.append(category);
        json.append(progress);
        json.append(transaction_id);
        json.append(properties);

        SplytResponse resp = Network::Call("datacollector_updateTransaction", json, context);
        return HandleResponse("datacollector_updateTransaction", resp);
    }

    SplytResponse EndTransaction(std::string category, std::string result, std::string transaction_id, std::string context, std::string user_id, std::string device_id, Json::Value properties)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        splyt::AppendUD(&json, user_id, device_id);
        json.append(category);
        json.append(result);
        json.append(transaction_id);
        json.append(properties);

        SplytResponse resp = Network::Call("datacollector_endTransaction", json, context);
        return HandleResponse("datacollector_endTransaction", resp);
    }
}
