#ifndef SPLYT_HTTP_INTERFACES_H_
#define SPLYT_HTTP_INTERFACES_H_

#include <curl/curl.h>
#include <string>
#include <stdexcept>

#include "util/log.h"

namespace splyt
{
    class HttpInterface
    {
        protected:
            HttpInterface(){}

        public:
            virtual std::string Post(std::string url, std::string path, std::string headers[], int header_count, std::string content) = 0;
    };

    class CurlHttpInterface : public splyt::HttpInterface
    {
        private:
            static size_t write_to_string(void *ptr, size_t size, size_t count, void *stream);

        public:
            CurlHttpInterface();

            virtual std::string Post(std::string url, std::string path, std::string headers[], int header_count, std::string content);
    };
}
#endif  // SPLYT_HTTP_INTERFACES_H_