#ifndef SPLYT_H_
#define SPLYT_H_

#include <stddef.h>
#include <ctime>
#include <string>
#include <json/json.h>

#include "util/util.h"
#include "util/config.h"
#include "util/log.h"
#include "network/splyt_response.h"
#include "network/http_interfaces.h"
#include "api/tuning.h"
#include "api/transaction.h"
#include "network/network.h"

namespace splytapi
{
    enum Error {
        kErrorSuccess = 0,
        kErrorGeneric = -1,
        kErrorNotInitialized = -2,
        kErrorNotFound = -3,
        kErrorInvalidArgs = -4,
        kErrorMissingId = -5,
        kErrorRequestTimedOut = -6,
        kErrorUnknown = -7
    };

    class Network;
    class Transaction;
    class Tuning;

    class Splyt
    {
        private:
            Network* network;

        public:
            std::string customer_id;
            std::string user_id;
            std::string device_id;
            Transaction* transaction;
            Tuning* tuning;

            ~Splyt();

            //Helper function to append user_id and device_id to API calls.
            void AppendUD(Json::Value* json, std::string user_id, std::string device_id);

            void InitNetwork(HttpInterface* httpint);

            Network* GetNetwork()
            {
                return network;
            }

            SplytResponse HandleResponse(std::string type, SplytResponse resp);

            /** Create a new user.

                @param std::string user_id
                @param std::string context

                @return SplytResponse
                @throws std::runtime_error
            */
            SplytResponse NewUser(std::string user_i, std::string context);

            /** Create a new device.

                @param std::string device_id
                @param std::string context

                @return SplytResponse
                @throws std::runtime_error
            */
            SplytResponse NewDevice(std::string device_id, std::string context);

            /** Create a new user if it does not already exist.

                @param std::string user_id
                @param std::string context

                @return SplytResponse
                @throws std::runtime_error
            */
            SplytResponse NewUserChecked(std::string user_id, std::string context);

            /** Create a new device if it does not already exist.

                @param std::string device_id
                @param std::string context

                @return SplytResponse
                @throws std::runtime_error
            */
            SplytResponse NewDeviceChecked(std::string device_id, std::string context);
    };

    /** Initialize the Splyt SDK.
        This function must be called first to use any functions provided in the SDK.

        NOTE: user_id and device_id are optional, but you must at least pass one or the other.
        @param std::string customer_id - Customer ID provided by Splyt.
        @param std::string user_id
        @param std::string device_id
        @param std::string context
        @param HttpInterface httpint - Optional HttpInterface used for HTTP requests. If one is not passed, it will use the default CurlHttpInterface.

        @returns Splyt - An instance of the Splyt API.
        @throws std::runtime_error
    */
    extern Splyt* Init(std::string customer_id, std::string user_id, std::string device_id, std::string context);
    extern Splyt* Init(std::string customer_id, std::string user_id, std::string device_id, std::string context, HttpInterface* httpint);
}
#endif  // SPLYT_H_