#include "api/tuning.h"

namespace splyt
{
    SplytResponse Tuning::GetTuningVal(std::string name, std::string entity_id, EntityType entity_type, NetworkCallback callback)
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