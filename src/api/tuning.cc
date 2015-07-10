#include "api/tuning.h"

namespace splyt
{
    std::map<std::string, TuningValue*> Tuning::value_cache;
    long Tuning::getallval_cache_ttl = 0L;

    void Tuning::CacheValues(Json::Value values, bool getallc)
    {
        long cache_time = Util::GetTimestamp() + Config::kTuningCacheTtl;

        if (getallc) {
            Tuning::getallval_cache_ttl = cache_time;
        }

        Log::Info("CACHING TUNING VALUES: " + values.toStyledString());
        for(Json::Value::iterator it = values.begin(); it != values.end(); ++it)
        {
            Json::Value value = (*it);
            std::string skey = it.key().asString();

            TuningValue* tval = new TuningValue(value.asString(), cache_time);
            value_cache[skey] = tval;
        }
    }

    SplytResponse Tuning::GetAllValues(std::string entity_id, EntityType entity_type)
    {
        long curtime = Util::GetTimestamp();

        if (curtime > Tuning::getallval_cache_ttl) {
            Log::Info("Get All Value cache has expired.");
            Json::Value json;

            std::string ts = Util::GetTimestampStr();
            json.append(ts);
            json.append(ts);
            json.append(entity_id);
            std::string entity_type_string = Tuning::GetEntityTypeString(entity_type);
            json.append(entity_type_string);

            SplytResponse resp = Network::Call("tuner_getAllValues", json);
            resp.SetContent(resp.GetContent()["value"]);
            Tuning::CacheValues(resp.GetContent(), true);

            return resp;
        }

        Log::Info("Get All Value cache has NOT expired.");

        SplytResponse resp(true);

        Json::Value root;
        for(std::map<std::string, TuningValue*>::iterator it = value_cache.begin(); it != value_cache.end(); it++) {
            root[it->first] = it->second->GetValue();
        }
        resp.SetContent(root);

        Log::Info("GET: " + resp.GetContent().toStyledString());
        return resp;
    }

    SplytResponse Tuning::GetValue(std::string name, std::string entity_id, EntityType entity_type)
    {
        long curtime = Util::GetTimestamp();
        TuningValue* v = value_cache.find(name)->second;
        long ttl = 0L;

        if (v != NULL) {
            ttl = v->GetTtl();
        }

        if (curtime >= ttl) {
            Log::Info("Value cache has expired.");

            Json::Value json;

            std::string ts = Util::GetTimestampStr();
            json.append(ts);
            json.append(ts);
            json.append(name);
            json.append(entity_id);
            std::string entity_type_string = Tuning::GetEntityTypeString(entity_type);
            json.append(entity_type_string);

            SplytResponse resp = Network::Call("tuner_getValue", json);
            resp.SetContent(resp.GetContent()["value"]);
            Tuning::CacheValues(resp.GetContent());

            return resp;
        }

        Log::Info("Value cache has NOT expired.");
        SplytResponse resp(true);

        Json::Value root;
        root[name] = v->GetValue();
        resp.SetContent(root);

        Log::Info("GET: " + resp.GetContent().toStyledString());

        return resp;
    }

    SplytResponse Tuning::RecordValue(std::string name, std::string default_value, std::string user_id, std::string device_id)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        splyt::AppendUD(&json, user_id, device_id);
        json.append(name);
        json.append(default_value);

        SplytResponse resp = Network::Call("tuner_recordUsed", json);
        return resp;
    }

    std::string Tuning::GetEntityTypeString(EntityType entity_type)
    {
        switch (entity_type) {
            case kEntityTypeUser:
                return "USER";
            case kEntityTypeDevice:
                return "DEVICE";
        }
        return "UNKNOWN";
    }
}