#include "network/http_interfaces.h"

namespace splyt
{
    CurlHttpInterface::CurlHttpInterface()
    {
        curl_global_init(CURL_GLOBAL_ALL);
    }

    size_t CurlHttpInterface::write_to_string(void *ptr, size_t size, size_t count, void *stream) {
        ((std::string*)stream)->append((char*)ptr, 0, size*count);
        return size*count;
    }

    std::string CurlHttpInterface::Post(std::string url, std::string path, std::string headers[], int header_count, std::string content)
    {
        CURL* curl;
        CURLcode res;
        curl = curl_easy_init();

        std::string response;

        if (curl) {
            //Construct URL.
            std::string full_url = url + path;
            splyt::Log::Info(full_url);
            splyt::Log::Info("REQUEST: " + content, false);

            //Construct headers.
            struct curl_slist* headerchunk = NULL;
            for(int i = 0; i < header_count; i++)
            {
                headerchunk = curl_slist_append(headerchunk, headers[i].c_str());
            }

            curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerchunk);
            curl_easy_setopt(curl, CURLOPT_POST, 1);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); //CURL verbose debug.

            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                throw std::runtime_error(curl_easy_strerror(res));
            }

            curl_easy_cleanup(curl);
            curl_slist_free_all(headerchunk);
        }

        return response;
    }
}