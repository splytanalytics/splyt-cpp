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

    class Tuning
    {
        private:
            static std::string GetEntityTypeString(EntityType entity_type);

        public:
            static SplytResponse GetAllValues(std::string entity_id, EntityType entity_type, NetworkCallback callback = NULL);
            static SplytResponse GetValue(std::string name, std::string entity_id, EntityType entity_type, NetworkCallback callback = NULL);
            static SplytResponse RecordValue(std::string name, std::string user_id, std::string device_id, std::string default_value, NetworkCallback callback = NULL);
            static SplytResponse Refresh(std::string user_id, std::string device_id, NetworkCallback callback = NULL);
    };
}
#endif  // SPLYT_TUNING_H_