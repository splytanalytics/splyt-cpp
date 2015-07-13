#include "api/transaction.h"

namespace splytapi
{
    Transaction::Transaction(Splyt* sp)
    {
        s = sp;
    }

    SplytResponse Transaction::Begin(std::string category, int timeout, std::string transaction_id, std::string context, std::string user_id, std::string device_id, Json::Value properties)
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

    SplytResponse Transaction::Update(std::string category, int progress, std::string transaction_id, std::string context, std::string user_id, std::string device_id, Json::Value properties)
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

    SplytResponse Transaction::End(std::string category, std::string result, std::string transaction_id, std::string context, std::string user_id, std::string device_id, Json::Value properties)
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
}