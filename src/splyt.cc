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

    Json::Value NewUser(std::string user_id)
    {
        Json::Value json;

        std::string ts = GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(Json::Value::null);

        NetworkResponse resp = Network::Call("datacollector_newUser", json);
        if (!resp.IsSuccessful()) {
            throw std::runtime_error("Splyt Error: " + resp.GetErrorMessage());
        }

        return resp.GetContent()["datacollector_newUser"]["data"];
    }

    Json::Value NewDevice(std::string device_id)
    {
        Json::Value json;

        std::string ts = GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(Json::Value::null);
        json.append(device_id);

        NetworkResponse resp = Network::Call("datacollector_newDevice", json);
        if (!resp.IsSuccessful()) {
            throw std::runtime_error("Splyt Error: " + resp.GetErrorMessage());
        }

        return resp.GetContent()["datacollector_newDevice"]["data"];
    }

    Json::Value NewUserChecked(std::string user_id)
    {
        Json::Value json;

        std::string ts = GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(Json::Value::null);

        NetworkResponse resp = Network::Call("datacollector_newUserChecked", json);
        if (!resp.IsSuccessful()) {
            throw std::runtime_error("Splyt Error: " + resp.GetErrorMessage());
        }

        return resp.GetContent()["datacollector_newUserChecked"]["data"];
    }

    Json::Value NewDeviceChecked(std::string device_id)
    {
        Json::Value json;

        std::string ts = GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(Json::Value::null);
        json.append(device_id);

        NetworkResponse resp = Network::Call("datacollector_newDeviceChecked", json);
        if (!resp.IsSuccessful()) {
            throw std::runtime_error("Splyt Error: " + resp.GetErrorMessage());
        }

        return resp.GetContent()["datacollector_newDeviceChecked"]["data"];
    }

    Json::Value BeginTransaction(std::string user_id, std::string device_id, std::string category, int timeout, std::string transaction_id, Json::Value properties)
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

        NetworkResponse resp = Network::Call("datacollector_beginTransaction", json);
        if (!resp.IsSuccessful()) {
            throw std::runtime_error("Splyt Error: " + resp.GetErrorMessage());
        }

        return resp.GetContent()["datacollector_beginTransaction"]["data"];
    }

    Json::Value UpdateTransaction(std::string user_id, std::string device_id, std::string category, int progress, std::string transaction_id, Json::Value properties)
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

        NetworkResponse resp = Network::Call("datacollector_updateTransaction", json);
        if (!resp.IsSuccessful()) {
            throw std::runtime_error("Splyt Error: " + resp.GetErrorMessage());
        }

        return resp.GetContent()["datacollector_updateTransaction"]["data"];
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
