#include <curl/curl.h>
#include <stdio.h>
#include <string>

#include "splyt.h"
#include "util/log.h"
#include "network/network.h"

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
                    //Construct URL.
                    std::string full_url = url + path;
                    splyt::Log::Info(full_url);
                    splyt::Log::Info(content);

                    //Construct headers.
                    struct curl_slist* headerchunk = NULL;
                    headerchunk = curl_slist_append(headerchunk, "ssf-use-positional-post-params: true");
                    headerchunk = curl_slist_append(headerchunk, "ssf-contents-not-url-encoded: true");

                    curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerchunk);
                    curl_easy_setopt(curl, CURLOPT_POST, 1);
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content.c_str());

                    res = curl_easy_perform(curl);

                    if (res != CURLE_OK) {
                        splyt::Log::Error(curl_easy_strerror(res));
                    }
                    printf("\n");

                    curl_easy_cleanup(curl);
                }
            }
    };
}

void SplytCallback(int var)
{
    splyt::Log::Info("Callback tests.");
}

int main ()
{
    splyt::Log::Info("Starting tests.");

    test::CurlHttpInterface httpint;
    splyt::Init(httpint, "knetik-bubblepop-test", "testuser", "");

    splyt::Network::Call("datacollector_beginTransaction", "[1435865035.85,1435865035.85,1435865035593,\"964cb8b8e445dc05a4f8421b0a13d24c\",\"session\",\"ANY\",3600,null,{}]", &SplytCallback);

    return 0;
}