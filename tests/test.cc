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
    splytapi::Log::Info("SLEEPING...");

    #ifdef _WIN32
        Sleep(value);
    #else
        usleep(value * 1000);
    #endif
}

int main ()
{
    splytapi::Config::kNetworkHost = "https://data.splyt.com";
    splytapi::Config::kTuningCacheTtl = 10000;

    splytapi::Log::Info("Starting tests...");

    //Initialize the Splyt API.
    splytapi::Splyt* splyt = splytapi::Init("knetik-bubblepop-test", "testuser", "testdevice", "testContext");

    splyt->transaction->Begin("testtransaction", "testcategory", 3600, "testContext");

    Sleep(2000);

    splyt->tuning->GetValue("testvar", "testuser", splytapi::kEntityTypeUser);

    splyt->tuning->GetAllValues("testuser", splytapi::kEntityTypeUser);

    Sleep(10000);

    splyt->tuning->GetAllValues("testuser", splytapi::kEntityTypeUser);

    splyt->UpdateUserState("testuser", "testContext", Json::Value::null);

    splyt->UpdateDeviceState("testdevice", "testContext", Json::Value::null);

    splyt->UpdateCollection("testcollection", 100, -10, false, "testContext");

    splyt->transaction->End("testtransaction", "testcategory", "success", "testContext");

    //Clean up memory, the API automatically handles the resources it uses.
    splytapi::Log::Info("Cleaning up memory...");
    delete splyt;
    splyt = NULL;
    splytapi::Log::Info("Done.");

    return 0;
}