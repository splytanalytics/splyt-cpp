#include <curl/curl.h>
#include <stdio.h>
#include <string>

#include "splyt.h"
#include "util/log.h"
#include "network/network.h"

namespace test
{
    /* Extends HttpInterface to allow the SDK to make HTTP calls
       with CURL for Linux.
    */
    class CurlHttpInterface : public splyt::HttpInterface
    {
        private:
            CURL* curl;
            CURLcode res;

        public:
            CurlHttpInterface()
            {
                curl_global_init(CURL_GLOBAL_ALL);

                curl = curl_easy_init();
            }

            static size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
                ((std::string*)stream)->append((char*)ptr, 0, size*count);
                return size*count;
            }

            virtual std::string Post(std::string url, std::string path, std::string headers[], int header_count, std::string content)
            {
                if (curl) {
                    //Construct URL.
                    std::string full_url = url + path;
                    splyt::Log::Info(full_url);
                    splyt::Log::Info(content);

                    //Construct headers.
                    struct curl_slist* headerchunk = NULL;
                    for(int i = 0; i < header_count; i++)
                    {
                        headerchunk = curl_slist_append(headerchunk, headers[i].c_str());
                    }

                    std::string response;
                    curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerchunk);
                    curl_easy_setopt(curl, CURLOPT_POST, 1);
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content.c_str());
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
                    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); //CURL verbose debug.

                    res = curl_easy_perform(curl);

                    if (res != CURLE_OK) {
                        splyt::Log::Error("CURL ERROR");
                        splyt::Log::Error(curl_easy_strerror(res));
                    }

                    //curl_easy_cleanup(curl);
                    curl_slist_free_all(headerchunk);

                    return response;
                }
            }
    };
}

void SplytCallback(Json::Value)
{
    splyt::Log::Info("Callback tests.");
}

int main ()
{
    splyt::Log::Info("Starting tests.");

    test::CurlHttpInterface httpint;
    splyt::Init(httpint, "knetik-bubblepop-test", "testuser", "");

    splyt::NewUserChecked("what");

    Json::Value properties;
    splyt::BeginTransaction("testuser,", "", "test_cat", 3600, "", properties);

    splyt::UpdateTransaction("testuer", "", "test_cat", 24, "", properties);

    splyt::EndTransaction("testuser", "", "test_cat", "success", "", properties);
    return 0;
}