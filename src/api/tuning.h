#ifndef SPLYT_TUNING_H_
#define SPLYT_TUNING_H_

#include "splyt.h"
#include "network/network.h"

namespace splyt
{
    enum EntityType {
        kEntityTypeUser,
        kEntityTypeDevice
    };

    class TuningValue
    {
        private:
            std::string value;
            long ttl;

        public:
            TuningValue(std::string val, long time)
            {
                value = val;
                ttl = time;
            }

            long GetTtl()
            {
                return ttl;
            }

            std::string GetValue()
            {
                return value;
            }
    };

    class Tuning
    {
        private:
            static std::map<std::string, TuningValue*> value_cache;
            static long getallval_cache_ttl;

            static std::string GetEntityTypeString(EntityType entity_type);
            static void CacheValues(Json::Value values, bool getallc = false);

        public:
            static SplytResponse GetAllValues(std::string entity_id, EntityType entity_type);
            static SplytResponse GetValue(std::string name, std::string entity_id, EntityType entity_type);
            static SplytResponse RecordValue(std::string name, std::string user_id, std::string device_id, std::string default_value);
            static SplytResponse Refresh(std::string user_id, std::string device_id);

            friend class Network;
    };
}
#endif  // SPLYT_TUNING_H_