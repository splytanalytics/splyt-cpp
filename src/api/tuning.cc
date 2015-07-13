#include "api/tuning.h"

namespace splytapi
{
    Tuning::Tuning(Splyt* sp, Json::Value json)
    {
        s = sp;

        CacheValues(json["devicetuning"], kEntityTypeUser, true);
        CacheValues(json["usertuning"], kEntityTypeDevice, true);
    }

    Tuning::~Tuning()
    {
        splytapi::Log::Info("Freeing tuning memory.");
    }

    void Tuning::CacheValues(Json::Value values, EntityType entity_type, bool getallc)
    {
        long cache_time = Util::GetTimestamp() + Config::kTuningCacheTtl;

        if (getallc) {
            Tuning::getallval_cache_ttl = cache_time;
        }

        Log::Info("CACHING TUNING VALUES: " + Tuning::GetEntityTypeString(entity_type) + " " + values.toStyledString());
        for(Json::Value::iterator it = values.begin(); it != values.end(); ++it)
        {
            Json::Value value = (*it);
            std::string skey = it.key().asString();

            TuningValue* tval = new TuningValue(value.asString(), cache_time);
            if(entity_type == kEntityTypeUser) {
                user_value_cache[skey] = tval;
            } else {
                device_value_cache[skey] = tval;
            }
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

            SplytResponse resp = s->GetNetwork()->Call("tuner_getAllValues", json);
            resp.SetContent(resp.GetContent()["value"]);
            CacheValues(resp.GetContent(), entity_type, true);

            return resp;
        }

        Log::Info("Get All Value cache has NOT expired.");

        SplytResponse resp(true);

        Json::Value root;
        if (entity_type == kEntityTypeUser) {
            for(std::map<std::string, TuningValue*>::iterator it = user_value_cache.begin(); it != user_value_cache.end(); it++) {
                root[it->first] = it->second->GetValue();
            }
        } else {
            for(std::map<std::string, TuningValue*>::iterator it = device_value_cache.begin(); it != device_value_cache.end(); it++) {
                root[it->first] = it->second->GetValue();
            }
        }

        resp.SetContent(root);

        Log::Info("GET: " + Tuning::GetEntityTypeString(entity_type) + " " + resp.GetContent().toStyledString());
        return resp;
    }

    SplytResponse Tuning::GetValue(std::string name, std::string entity_id, EntityType entity_type)
    {
        long curtime = Util::GetTimestamp();
        TuningValue* v = NULL;
        if (entity_type == kEntityTypeUser) {
            v = user_value_cache.find(name)->second;
        } else {
            v = device_value_cache.find(name)->second;
        }
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

            SplytResponse resp = s->GetNetwork()->Call("tuner_getValue", json);
            resp.SetContent(resp.GetContent()["value"]);
            CacheValues(resp.GetContent(), entity_type);

            return resp;
        }

        Log::Info("Value cache has NOT expired.");
        SplytResponse resp(true);

        Json::Value root;
        root[name] = v->GetValue();
        resp.SetContent(root);

        Log::Info("GET: " + Tuning::GetEntityTypeString(entity_type) + " " + resp.GetContent().toStyledString());

        return resp;
    }

    SplytResponse Tuning::RecordValue(std::string name, std::string default_value, std::string user_id, std::string device_id)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        s->AppendUD(&json, user_id, device_id);
        json.append(name);
        json.append(default_value);

        SplytResponse resp = s->GetNetwork()->Call("tuner_recordUsed", json);
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