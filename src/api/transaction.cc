/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#include "api/transaction.h"

namespace splytapi
{
    Transaction::Transaction(Splyt* sp)
    {
        s = sp;
    }

    SplytResponse Transaction::Begin(std::string transaction_id, std::string category, int timeout, std::string context, std::string user_id, std::string device_id, Json::Value properties)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        s->AppendUD(&json, user_id, device_id);
        json.append(category);
        json.append("TXN");
        json.append(timeout);
        json.append(transaction_id);
        json.append(properties);

        SplytResponse resp = s->GetNetwork()->Call("datacollector_beginTransaction", json, context);
        return s->HandleResponse("datacollector_beginTransaction", resp);
    }
    void Transaction::BeginAsync(NetworkCallback callback, std::string transaction_id, std::string category, int timeout, std::string context, std::string user_id, std::string device_id, Json::Value properties)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        s->AppendUD(&json, user_id, device_id);
        json.append(category);
        json.append("TXN");
        json.append(timeout);
        json.append(transaction_id);
        json.append(properties);

        s->thread_manager->PushTask(callback, "datacollector_beginTransaction", json, context);
    }

    SplytResponse Transaction::Update(std::string transaction_id, std::string category, double progress, std::string context, std::string user_id, std::string device_id, Json::Value properties)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        s->AppendUD(&json, user_id, device_id);
        json.append(category);
        json.append(progress);
        json.append(transaction_id);
        json.append(properties);

        SplytResponse resp = s->GetNetwork()->Call("datacollector_updateTransaction", json, context);
        return s->HandleResponse("datacollector_updateTransaction", resp);
    }
    void Transaction::UpdateAsync(NetworkCallback callback, std::string transaction_id, std::string category, double progress, std::string context, std::string user_id, std::string device_id, Json::Value properties)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        s->AppendUD(&json, user_id, device_id);
        json.append(category);
        json.append(progress);
        json.append(transaction_id);
        json.append(properties);

        s->thread_manager->PushTask(callback, "datacollector_updateTransaction", json, context);
    }

    SplytResponse Transaction::End(std::string transaction_id, std::string category, std::string result, std::string context, std::string user_id, std::string device_id, Json::Value properties)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        s->AppendUD(&json, user_id, device_id);
        json.append(category);
        json.append(result);
        json.append(transaction_id);
        json.append(properties);

        SplytResponse resp = s->GetNetwork()->Call("datacollector_endTransaction", json, context);
        return s->HandleResponse("datacollector_endTransaction", resp);
    }
    void Transaction::EndAsync(NetworkCallback callback, std::string transaction_id, std::string category, std::string result, std::string context, std::string user_id, std::string device_id, Json::Value properties)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        s->AppendUD(&json, user_id, device_id);
        json.append(category);
        json.append(result);
        json.append(transaction_id);
        json.append(properties);

        s->thread_manager->PushTask(callback, "datacollector_endTransaction", json, context);
    }

    SplytResponse Transaction::BeginEnd(std::string transaction_id, std::string category, std::string result, std::string context, std::string user_id, std::string device_id, Json::Value properties)
    {
        this->Begin(transaction_id, category, 1000, context, user_id, device_id, Json::Value::null);
        return this->End(transaction_id, category, result, context, user_id, device_id, properties);
    }
    void Transaction::BeginEndAsync(NetworkCallback callback, std::string transaction_id, std::string category, std::string result, std::string context, std::string user_id, std::string device_id, Json::Value properties)
    {
        this->BeginAsync(NULL, transaction_id, category, 1000, context, user_id, device_id, Json::Value::null);
        this->EndAsync(callback, transaction_id, category, result, context, user_id, device_id, properties);
    }
}