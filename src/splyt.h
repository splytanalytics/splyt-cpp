#ifndef SPLYT_H_
#define SPLYT_H_

#include <stddef.h>
#include <string>

namespace splyt
{
    class HttpInterface
    {
        public:
            virtual ~HttpInterface() {}
            virtual void Post(std::string url, std::string path, std::string content) {}
    };

    extern void Init(HttpInterface& httpint);
}
#endif  // SPLYT_H_