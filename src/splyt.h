#ifndef SPLYT_H_
#define SPLYT_H_

#define SPLYT_QPARM_WSVERSION "ssf_ws_version=4"
#define SPLYT_QPARM_OUTPUT "ssf_output=json"

#include <stddef.h>
#include <ctime>
#include <string>
#include <json/json.h>

#include "util/log.h"

namespace splyt
{
    typedef enum Error {
        Error_Success = 0,
        Error_Generic = -1,
        Error_NotInitialized = -2,
        Error_AlreadyInitialized = -3,
        Error_InvalidArgs = -4,
        Error_MissingId = -5,
        Error_RequestTimedOut = -6,
        Error_Unknown = -7
    } Error;

    class HttpInterface
    {
        public:
            virtual ~HttpInterface() {}
            virtual std::string Post(std::string url, std::string path, std::string headers[], int header_count, std::string content) {}
    };

    extern bool initialized;
    extern std::string customer_id;
    extern std::string user_id;
    extern std::string device_id;

    extern void Init(HttpInterface& httpint, std::string customer_id, std::string user_id, std::string device_id);

    static std::string GetTimestampStr()
    {
        std::stringstream strm;
        strm << (std::time(0) * 1000);
        return strm.str();
    }

    extern Json::Value BeginTransaction(std::string user_id, std::string device_id, std::string category, std::string timeout_mode, int timeout, std::string transaction_id, Json::Value properties);
    extern Json::Value EndTransaction(std::string user_id, std::string device_id, std::string category, std::string result, std::string transaction_id, Json::Value properties);
}
#endif  // SPLYT_H_