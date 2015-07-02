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

                    //Construct headers.
                    struct curl_slist *chunk = NULL;
                    chunk = curl_slist_append(chunk, "ssf-use-positional-post-params: true");
                    chunk = curl_slist_append(chunk, "ssf-contents-not-url-encoded: true");
                    chunk = curl_slist_append(chunk, "Content-Type: text/json");

                    curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
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

int main ()
{
    splyt::Log::Info("Starting tests.");

    test::CurlHttpInterface httpint;
    splyt::Init(httpint, "splyt-bubblepop-test", "testuser", "");

    //Network::Call("datacollector_beginTransaction", "[1435865035.85,1435865035.85,1435865035593,'964cb8b8e445dc05a4f8421b0a13d24c','session','ANY',3600,null,{}]");
    splyt::Network::Call("application_init_with_entitystate_telem", "[1435865035.595,1435865035.595,1435865035593,'964cb8b8e445dc05a4f8421b0a13d24c',{'gender':'male','referral':'facebook','myInitProp2':'cat2'}, {'myInitDevProp':'kitty2','browser':'Chrome','browserversion':'43.0.2357.130','osname':'Linux','description':'Chrome 43.0.2357.130 on Linux 64-bit','language':'en-us'}]");

    return 0;
}