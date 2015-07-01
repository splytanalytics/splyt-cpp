#include <curl/curl.h>
#include <stdio.h>
#include <string>

#include "splyt.h"

namespace test
{
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

            void Post(std::string url, std::string path, std::string content)
            {
                if (curl) {
                    printf("Network post test.\n");

                    curl_easy_setopt(curl, CURLOPT_URL, (url + path).c_str());
                    curl_easy_setopt(curl, CURLOPT_POST, 1);
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content.c_str());

                    res = curl_easy_perform(curl);

                    if (res != CURLE_OK) {
                        fprintf(stderr, "post failed: %s\n", curl_easy_strerror(res));
                    }

                    curl_easy_cleanup(curl);
                }
            }
    };
}

int main ()
{
    printf("Starting test.\n");

    test::CurlHttpInterface httpint;
    splyt::Init(httpint);

    return 0;
}