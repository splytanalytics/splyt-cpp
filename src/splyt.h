#ifndef SPLYT_H_
#define SPLYT_H_

#include <stddef.h>
#include <ctime>
#include <string>
#include <json/json.h>

#include "util/util.h"
#include "util/config.h"
#include "util/log.h"
#include "network/network_defs.h"
#include "network/http_interfaces.h"
#include "api/tuning.h"

namespace splyt
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

    extern bool initialized;
    extern std::string customer_id;
    extern std::string user_id;
    extern std::string device_id;

    /** Initialize the Splyt SDK.
        This function must be called first to use any functions provided in the SDK.

        NOTE: user_id and device_id are optional, but you must at least pass one or the other.
        @param std::string customer_id - Customer ID provided by Splyt.
        @param std::string user_id
        @param std::string device_id
        @param std::string context
        @param HttpInterface httpint - Optional HttpInterface used for HTTP requests. If one is not passed, it will use the default CurlHttpInterface.

        @throws std::runtime_error
    */
    extern void Init(std::string customer_id, std::string user_id, std::string device_id, std::string context);
    extern void Init(std::string customer_id, std::string user_id, std::string device_id, std::string context, HttpInterface* httpint);

    extern SplytResponse HandleResponse(std::string type, SplytResponse resp);

    /** Create a new user.

        @param std::string user_id
        @param std::string context

        @return SplytResponse
        @throws std::runtime_error
    */
    extern SplytResponse NewUser(std::string user_i, std::string context);

    /** Create a new device.

        @param std::string device_id
        @param std::string context

        @return SplytResponse
        @throws std::runtime_error
    */
    extern SplytResponse NewDevice(std::string device_id, std::string context);

    /** Create a new user if it does not already exist.

        @param std::string user_id
        @param std::string context

        @return SplytResponse
        @throws std::runtime_error
    */
    extern SplytResponse NewUserChecked(std::string user_id, std::string context);

    /** Create a new device if it does not already exist.

        @param std::string device_id
        @param std::string context

        @return SplytResponse
        @throws std::runtime_error
    */
    extern SplytResponse NewDeviceChecked(std::string device_id, std::string context);

    /** Begin a new transaction.

        NOTE: user_id and device_id are optional, but you must at least pass one or the other.
        @param std::string user_id
        @param std::string device_id
        @param std::string category
        @param int timeout
        @param std::string transaction_id
        @param std::string context
        @param Json::Value properties

        @return SplytResponse
        @throws std::runtime_error
    */
    extern SplytResponse BeginTransaction(std::string user_id, std::string device_id, std::string category, int timeout, std::string transaction_id, std::string context, Json::Value properties = Json::Value::null);

    /** Update an existing transaction.

        NOTE: user_id and device_id are optional, but you must at least pass one or the other.
        @param std::string user_id
        @param std::string device_id
        @param std::string category
        @param int progress
        @param std::string transaction_id
        @param std::string context
        @param Json::Value properties

        @return SplytResponse
        @throws std::runtime_error
    */
    extern SplytResponse UpdateTransaction(std::string user_id, std::string device_id, std::string category, int progress, std::string transaction_id, std::string context, Json::Value properties = Json::Value::null);

    /** End an existing transaction.

        NOTE: user_id and device_id are optional, but you must at least pass one or the other.
        @param std::string user_id
        @param std::string device_id
        @param std::string category
        @param std::string result
        @param std::string transaction_id
        @param std::string context
        @param Json::Value properties

        @return SplytResponse
        @throws std::runtime_error
    */
    extern SplytResponse EndTransaction(std::string user_id, std::string device_id, std::string category, std::string result, std::string transaction_id, std::string context, Json::Value properties = Json::Value::null);
}
#endif  // SPLYT_H_