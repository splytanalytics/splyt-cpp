#ifndef SPLYT_TUNING_H_
#define SPLYT_TUNING_H_

#include "splyt.h"
#include "network/network.h"

namespace splytapi
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
            Splyt* s;
            std::map< std::string, std::map<std::string, TuningValue*> > users_value_cache;
            std::map< std::string, std::map<std::string, TuningValue*> > devices_value_cache;
            long getallval_cache_ttl;

            std::string GetEntityTypeString(EntityType entity_type);
            void CacheValues(std::string entity_id, Json::Value values, EntityType entity_type, bool getallc = false);

        public:
            Tuning(Splyt* sp, Json::Value json);
            ~Tuning();

            /** Get all tuning values for an entity. These values are cached according to the kTuningCacheTtl variable.

                @param std::string entity_id
                @param EntityType entity_type

                @return SplytResponse
                @throws splyt_exception
            */
            SplytResponse GetAllValues(std::string entity_id, EntityType entity_type);

            /** Get a single tuning value. These values are cached according to the kTuningCacheTtl variable.

                @param std::string name
                @param std::string default_value - Value returned if the Splyt network is not available.
                @param std::string entity_id
                @param EntityType entity_type

                @return SplytResponse
                @throws splyt_exception
            */
            SplytResponse GetValue(std::string name, std::string default_value, std::string entity_id, EntityType entity_type);

            /** Record the use of a tuning value.

                @param std::string name
                @param std::string default_value
                @param std::string user_id - Optional.
                @param std::string device_id - Optional.

                @return SplytResponse
                @throws splyt_exception
            */
            SplytResponse RecordValue(std::string name, std::string default_value, std::string user_id = "", std::string device_id = "");

            friend class Network;
    };
}
#endif  // SPLYT_TUNING_H_