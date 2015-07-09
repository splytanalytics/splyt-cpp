#include "api/tuning.h"

namespace splyt
{
    SplytResponse Tuning::GetAllValues(std::string entity_id, EntityType entity_type, NetworkCallback callback)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(entity_id);
        std::string entity_type_string = Tuning::GetEntityTypeString(entity_type);
        json.append(entity_type_string);

        SplytResponse resp = Network::Call("tuner_getAllValues", json, callback);
        return resp;
    }

    SplytResponse Tuning::GetValue(std::string name, std::string entity_id, EntityType entity_type, NetworkCallback callback)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(name);
        json.append(entity_id);
        std::string entity_type_string = Tuning::GetEntityTypeString(entity_type);
        json.append(entity_type_string);

        SplytResponse resp = Network::Call("tuner_getValue", json, callback);
        return resp;
    }

    SplytResponse Tuning::RecordValue(std::string name, std::string user_id, std::string device_id, std::string default_value, NetworkCallback callback)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(device_id);
        json.append(name);
        json.append(default_value);

        SplytResponse resp = Network::Call("tuner_recordUsed", json, callback);
        return resp;
    }

    SplytResponse Tuning::Refresh(std::string user_id, std::string device_id, NetworkCallback callback)
    {
        Json::Value json;

        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(user_id);
        json.append(device_id);

        SplytResponse resp = Network::Call("tuner_refresh", json, callback);
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