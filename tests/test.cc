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

void Log(std::string s)
{
    std::cout << "TEST LOG: " + s << std::endl;
}

int main ()
{
    splytapi::Config::kDebugLog = true;
    splytapi::Config::kNetworkHost = "https://data.splyt.com";
    splytapi::Config::kTuningCacheTtl = 10000;

    Log("Starting tests...");

    //Initialize the Splyt API.
    splytapi::Splyt* splyt = splytapi::Init("knetik-bubblepop-test", "testuser", "testdevice", "testContext");

    try {
        splytapi::SplytResponse resp = splyt->transaction->Begin("testtransaction", "testcategory", 3600, "testContext");
        Log("TESTRESPONSE: " + resp.GetContent().toStyledString());
    } catch (splytapi::splyt_exception e) {
        Log(e.GetResponse().GetErrorMessage());
    }

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
    Log("Cleaning up memory...");
    delete splyt;
    splyt = NULL;
    Log("Done.");

    return 0;
}