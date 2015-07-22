/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
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
            splytapi::ThrowDummyResponseException("A customer ID is required.");
        }

        if(user_id.empty() && device_id.empty()) {
            splytapi::ThrowDummyResponseException("A user or device ID is required.");
        }

        if (Config::kTuningCacheTtl <= 0) {
            splytapi::ThrowDummyResponseException("Invalid tuning cache TTL.");
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
            resp.SetErrorMessage("Splyt Error: " + resp.GetErrorMessage());
            throw splytapi::splyt_exception(resp);
        }

        return resp;
    }

    SplytResponse Splyt::NewUser(std::string nuser_id, std::string ncontext)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(nuser_id);
        json.append(Json::Value::null);

        SplytResponse resp = network->Call("datacollector_newUser", json, ncontext);
        return HandleResponse("datacollector_newUser", resp);
    }

    SplytResponse Splyt::NewDevice(std::string ndevice_id, std::string ncontext)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(Json::Value::null);
        json.append(ndevice_id);

        SplytResponse resp = network->Call("datacollector_newDevice", json, ncontext);
        return HandleResponse("datacollector_newDevice", resp);
    }

    SplytResponse Splyt::NewUserChecked(std::string nuser_id, std::string ncontext)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(nuser_id);
        json.append(Json::Value::null);

        SplytResponse resp = network->Call("datacollector_newUserChecked", json, ncontext);
        return HandleResponse("datacollector_newUserChecked", resp);
    }

    SplytResponse Splyt::NewDeviceChecked(std::string ndevice_id, std::string ncontext)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(Json::Value::null);
        json.append(ndevice_id);

        SplytResponse resp = network->Call("datacollector_newDeviceChecked", json, ncontext);
        return HandleResponse("datacollector_newDeviceChecked", resp);
    }

    SplytResponse Splyt::UpdateUserState(std::string nuser_id, std::string ncontext, Json::Value nproperties)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(nuser_id);
        json.append(Json::Value::null);
        json.append(nproperties);

        SplytResponse resp = network->Call("datacollector_updateUserState", json, ncontext);
        return HandleResponse("datacollector_updateUserState", resp);
    }

    SplytResponse Splyt::UpdateDeviceState(std::string ndevice_id, std::string ncontext, Json::Value nproperties)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(Json::Value::null);
        json.append(ndevice_id);
        json.append(nproperties);

        SplytResponse resp = network->Call("datacollector_updateDeviceState", json, ncontext);
        return HandleResponse("datacollector_updateDeviceState", resp);
    }

    SplytResponse Splyt::UpdateCollection(std::string nname, double nbalance, double nbalance_delta, bool nis_currency, std::string ncontext, std::string nuser_id, std::string ndevice_id)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        AppendUD(&json, nuser_id, ndevice_id);
        json.append(nname);
        json.append(nbalance);
        json.append(nbalance_delta);
        json.append(nis_currency);

        SplytResponse resp = network->Call("datacollector_updateCollection", json, ncontext);
        return HandleResponse("datacollector_updateCollection", resp);
    }

    Splyt::~Splyt()
    {
        Log::Info("Freeing Splyt memory.");
        delete network;
        network = NULL;

        delete transaction;
        transaction = NULL;

        delete tuning;
        tuning = NULL;
        Log::Info("Splyt memory freed.");
    }

    void ThrowDummyResponseException(std::string s)
    {
        SplytResponse response(false);
        response.SetErrorMessage(s);
        response.SetContent(Json::Value::null);
        throw splytapi::splyt_exception(response);
    }
}