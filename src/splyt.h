#ifndef SPLYT_H_
#define SPLYT_H_

#define SPLYT_QPARM_WSVERSION "ssf_ws_version=4"
#define SPLYT_QPARM_OUTPUT "ssf_output=json"

#include <stddef.h>
#include <string>

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
            virtual void Post(std::string url, std::string path, std::string content) {}
    };

    extern std::string customer_id;
    extern std::string user_id;
    extern std::string device_id;

    extern void Init(HttpInterface& httpint, std::string customer_id, std::string user_id, std::string device_id);
}
#endif  // SPLYT_H_