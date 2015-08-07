/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#include "override_http_interface.h"

namespace splytapi
{
    OverrideHttpInterface::OverrideHttpInterface()
    {
        curl_global_init(CURL_GLOBAL_ALL);
    }

    size_t OverrideHttpInterface::write_to_string(void *ptr, size_t size, size_t count, void *stream) {
        ((std::string*)stream)->append((char*)ptr, 0, size*count);
        return size*count;
    }

    std::string OverrideHttpInterface::Post(std::string url, std::string path, std::string headers[], int header_count, std::string content, long timeout)
    {
        CURL* curl;
        CURLcode res;
        curl = curl_easy_init();

        std::string response;

        if (curl) {
            //Construct URL.
            std::string full_url = url + path;
            splytapi::Log::Info(full_url);
            splytapi::Log::Info("REQUEST: " + content, false);

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
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, Config::kNetworkEnableVerifyPeer);
            //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); //CURL verbose debug.

            res = curl_easy_perform(curl);
            long http_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

            if (res != CURLE_OK) {
                throw std::runtime_error(curl_easy_strerror(res));
            }

            if (http_code < 200 || http_code >= 300) {
                //throw std::runtime_error("HTTP Error Code: " + std::to_string(http_code));
            }

            curl_slist_free_all(headerchunk);
            curl_easy_cleanup(curl);
        }

        return response;
    }
}