#include <stdexcept>
#include <stdio.h>

#include "splyt.h"

namespace splytapi
{
    Splyt* Init(std::string customer_id, std::string user_id, std::string device_id, std::string context) {
        CurlHttpInterface* httpint = new CurlHttpInterface();
        return Init(customer_id, user_id, device_id, context, httpint);
    }

    Splyt* Init(std::string customer_id, std::string user_id, std::string device_id, std::string context, HttpInterface* httpint) {
        Log::Info("Splyt init.");

        if(customer_id.empty()) {
            throw std::runtime_error("A customer ID is required.");
        }

        if(user_id.empty() && device_id.empty()) {
            throw std::runtime_error("A user or device ID is required.");
        }

        Splyt* s = new Splyt();

        s->customer_id = customer_id;
        s->user_id = user_id;
        s->device_id = device_id;

        s->InitNetwork(httpint);

        return s;
    }

    void Splyt::InitNetwork(HttpInterface* httpint)
    {
        network = new Network(this);
        Json::Value json = network->Init(httpint);

        transaction = new Transaction(this);
        tuning = new Tuning(this, json);
    }

    void Splyt::AppendUD(Json::Value* json, std::string nuser_id, std::string ndevice_id)
    {
        if (nuser_id.empty()) {
            if (user_id.empty()) {
                json->append(Json::Value::null);
            } else {
                json->append(user_id);
            }
        } else {
            json->append(nuser_id);
        }

        if (ndevice_id.empty()) {
            if (device_id.empty()) {
                json->append(Json::Value::null);
            } else {
                json->append(device_id);
            }
        } else {
            json->append(ndevice_id);
        }
    }

    SplytResponse Splyt::HandleResponse(std::string type, SplytResponse resp)
    {
        if (!resp.IsSuccessful()) {
            throw std::runtime_error("Splyt Error: " + resp.GetErrorMessage());
        }

        return resp;
    }

    SplytResponse Splyt::NewUser(std::string user_id, std::string context)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(Json::Value::null);

        SplytResponse resp = network->Call("datacollector_newUser", json, context);
        return HandleResponse("datacollector_newUser", resp);
    }

    SplytResponse Splyt::NewDevice(std::string device_id, std::string context)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(Json::Value::null);
        json.append(device_id);

        SplytResponse resp = network->Call("datacollector_newDevice", json, context);
        return HandleResponse("datacollector_newDevice", resp);
    }

    SplytResponse Splyt::NewUserChecked(std::string user_id, std::string context)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(Json::Value::null);

        SplytResponse resp = network->Call("datacollector_newUserChecked", json, context);
        return HandleResponse("datacollector_newUserChecked", resp);
    }

    SplytResponse Splyt::NewDeviceChecked(std::string device_id, std::string context)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(Json::Value::null);
        json.append(device_id);

        SplytResponse resp = network->Call("datacollector_newDeviceChecked", json, context);
        return HandleResponse("datacollector_newDeviceChecked", resp);
    }

    SplytResponse Splyt::UpdateUserState(std::string user_id, std::string context, Json::Value properties)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(Json::Value::null);
        json.append(properties);

        SplytResponse resp = network->Call("datacollector_updateUserState", json, context);
        return HandleResponse("datacollector_updateUserState", resp);
    }

    SplytResponse Splyt::UpdateDeviceState(std::string device_id, std::string context, Json::Value properties)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(Json::Value::null);
        json.append(device_id);
        json.append(properties);

        SplytResponse resp = network->Call("datacollector_updateDeviceState", json, context);
        return HandleResponse("datacollector_updateDeviceState", resp);
    }

    SplytResponse Splyt::UpdateCollection(std::string name, double balance, double balance_delta, bool is_currency, std::string context, std::string user_id, std::string device_id)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        AppendUD(&json, user_id, device_id);
        json.append(name);
        json.append(balance);
        json.append(balance_delta);
        json.append(is_currency);

        SplytResponse resp = network->Call("datacollector_updateCollection", json, context);
        return HandleResponse("datacollector_updateCollection", resp);
    }

    Splyt::~Splyt()
    {
        delete network;
        network = NULL;

        delete transaction;
        transaction = NULL;

        delete tuning;
        tuning = NULL;
    }
}