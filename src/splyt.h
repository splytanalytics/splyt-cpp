#ifndef SPLYT_H_
#define SPLYT_H_

#include <stddef.h>
#include <ctime>
#include <string>
#include <json/json.h>

#include "util/config.h"
#include "util/log.h"

namespace splyt
{
    typedef enum Error {
        Error_Success = 0,
        Error_Generic = -1,
        Error_NotInitialized = -2,
        Error_NotFound = -3,
        Error_InvalidArgs = -4,
        Error_MissingId = -5,
        Error_RequestTimedOut = -6,
        Error_Unknown = -7
    } Error;

    class HttpInterface
    {
        protected:
            HttpInterface(){}

        public:
            virtual std::string Post(std::string url, std::string path, std::string headers[], int header_count, std::string content) = 0;
    };

    extern bool initialized;
    extern std::string customer_id;
    extern std::string user_id;
    extern std::string device_id;

    /** Initialize the Splyt SDK.
        This function must be called first to use any functions provided in the SDK.

        NOTE: user_id and device_id are optional, but you must at least pass one or the other.
        @param HttpInterface httpint - HttpInterface used for HTTP requests.
        @param std::string customer_id - Customer ID provided by Splyt.
        @param std::string user_id
        @param std::string device_id

        @throws std::runtime_error
    */
    extern void Init(HttpInterface& httpint, std::string customer_id, std::string user_id, std::string device_id);

    static std::string GetTimestampStr()
    {
        std::stringstream strm;
        strm << (std::time(0) * 1000);
        return strm.str();
    }

    /** Create a new user.

        @param std::string user_id

        @return Json::Value
        @throws std::runtime_error
    */
    extern Json::Value NewUser(std::string user_id);

    /** Create a new device.

        @param std::string device_id

        @return Json::Value
        @throws std::runtime_error
    */
    extern Json::Value NewDevice(std::string device_id);

    /** Create a new user if it does not already exist.

        @param std::string user_id

        @return Json::Value
        @throws std::runtime_error
    */
    extern Json::Value NewUserChecked(std::string user_id);

    /** Create a new device if it does not already exist.

        @param std::string device_id

        @return Json::Value
        @throws std::runtime_error
    */
    extern Json::Value NewDeviceChecked(std::string device_id);

    /** Begin a new transaction.

        NOTE: user_id and device_id are optional, but you must at least pass one or the other.
        @param std::string user_id
        @param std::string device_id
        @param std::string category
        @param int timeout
        @param std::string transaction_id
        @param Json::Value properties

        @return Json::Value
        @throws std::runtime_error
    */
    extern Json::Value BeginTransaction(std::string user_id, std::string device_id, std::string category, int timeout, std::string transaction_id, Json::Value properties);

    /** Update an existing transaction.

        NOTE: user_id and device_id are optional, but you must at least pass one or the other.
        @param std::string user_id
        @param std::string device_id
        @param std::string category
        @param int progress
        @param std::string transaction_id
        @param Json::Value properties

        @return Json::Value
        @throws std::runtime_error
    */
    extern Json::Value UpdateTransaction(std::string user_id, std::string device_id, std::string category, int progress, std::string transaction_id, Json::Value properties);

    /** End an existing transaction.

        NOTE: user_id and device_id are optional, but you must at least pass one or the other.
        @param std::string user_id
        @param std::string device_id
        @param std::string category
        @param std::string result
        @param std::string transaction_id
        @param Json::Value properties

        @return Json::Value
        @throws std::runtime_error
    */
    extern Json::Value EndTransaction(std::string user_id, std::string device_id, std::string category, std::string result, std::string transaction_id, Json::Value properties);
}
#endif  // SPLYT_H_