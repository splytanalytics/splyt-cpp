#include <stdio.h>
#include <string>

#include "splyt.h"
#include "util/log.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

//Sleep function for testing caching.
void Sleep(long value)
{
    splyt::Log::Info("SLEEPING...");

    #ifdef _WIN32
        Sleep(value);
    #else
        usleep(value * 1000);
    #endif
}

int main ()
{
    splyt::Config::kNetworkHost = "https://data.splyt.com";
    splyt::Config::kTuningCacheTtl = 10000;

    splyt::Log::Info("Starting tests.");

    splyt::Init("knetik-bubblepop-test", "testuser", "testdevice", "testContext");

    splyt::Transaction::Begin("test_cat", 3600, "", "testContext");

    Sleep(2000);

    splyt::Tuning::GetValue("testvar", "testuser", splyt::kEntityTypeUser);

    splyt::Tuning::GetAllValues("testuser", splyt::kEntityTypeUser);

    Sleep(10000);

    splyt::Tuning::GetAllValues("testuser", splyt::kEntityTypeUser);

    return 0;
}