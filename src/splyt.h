/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#ifndef SPLYT_H_
#define SPLYT_H_

#include <cstdio>
#include <cstddef>
#include <stdexcept>
#include <ctime>
#include <string>
#include <json/json.h>

#include "util/util.h"
#include "util/config.h"
#include "util/log.h"
#include "network/splyt_response.h"
#include "util/splyt_exception.h"
#include "network/http_interface.h"
#include "override_http_interface.h"
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
                @throws splyt_exception
            */
            SplytResponse NewUser(std::string user_id, std::string context);

            /** Create a new device.

                @param std::string device_id
                @param std::string context

                @return SplytResponse
                @throws splyt_exception
            */
            SplytResponse NewDevice(std::string device_id, std::string context);

            /** Create a new user if it does not already exist.

                @param std::string user_id
                @param std::string context

                @return SplytResponse
                @throws splyt_exception
            */
            SplytResponse NewUserChecked(std::string user_id, std::string context);

            /** Create a new device if it does not already exist.

                @param std::string device_id
                @param std::string context

                @return SplytResponse
                @throws splyt_exception
            */
            SplytResponse NewDeviceChecked(std::string device_id, std::string context);

            /** Update user state.

                @param std::string user_id
                @param std::string context
                @param Json::Value properties

                @return SplytResponse
                @throws splyt_exception
            */
            SplytResponse UpdateUserState(std::string user_id, std::string context, Json::Value properties = Json::Value::null);

            /** Update device state.

                @param std::string device_id
                @param std::string context
                @param Json::Value properties

                @return SplytResponse
                @throws splyt_exception
            */
            SplytResponse UpdateDeviceState(std::string device_id, std::string context, Json::Value properties = Json::Value::null);

            /** Update collection, used for purchases.

                @param std::string name
                @param double balance
                @param balance_delta
                @param bool is_currency
                @param std::string context
                @param std::string user_id - Optional.
                @param std::string device_id - Optional.

                @return SplytResponse
                @throws splyt_exception
            */
            SplytResponse UpdateCollection(std::string name, double balance, double balance_delta, bool is_currency, std::string context, std::string user_id = "", std::string device_id = "");
    };

    /** Initialize the Splyt SDK.
        This function must be called first to use any functions provided in the SDK.

        NOTE: user_id and device_id are optional, but you must at least pass one or the other.
        @param std::string customer_id - Customer ID provided by Splyt.
        @param std::string user_id
        @param std::string device_id
        @param std::string context - Context of this API call.
        @param HttpInterface httpint - Optional HttpInterface used for HTTP requests. If one is not passed, it will use the default CurlHttpInterface.

        @returns Splyt - An instance of the Splyt API.
        @throws splyt_exception
    */
    extern Splyt* Init(std::string customer_id, std::string user_id, std::string device_id, std::string context);
    extern Splyt* Init(std::string customer_id, std::string user_id, std::string device_id, std::string context, HttpInterface* httpint);

    //Helper function to throw SplytException.
    extern void ThrowDummyResponseException(std::string s);
}
#endif  // SPLYT_H_