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
            static SplytResponse GetTuningVal(std::string name, std::string entity_id, EntityType entity_type, NetworkCallback callback = NULL);
    };
}
#endif  // SPLYT_TUNING_H_