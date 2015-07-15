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

template <typename T>
std::string to_string(T value)
{
  std::ostringstream os ;
  os << value ;
  return os.str() ;
}

void Log(std::string s)
{
    std::cout << "TEST LOG: " + s << std::endl;
}

//Test functions.
splytapi::Splyt* InitSplyt()
{
    //Initialize the Splyt API.
    splytapi::Splyt* splyt = splytapi::Init("knetik-bubblepop-test", "testuser", "testdevice", "testContext");
    return splyt;
}

void AssertJsonStringEquals(Json::Value json, std::string key, std::string value, int line)
{
    std::string test = json[key].asString();
    if (test != value) {
        throw std::runtime_error("LINE " + to_string(line) + " ASSERT JSON STRING EQUALS FAILED: key:\"" + key + "\" != value:\"" + value + "\": \"" + test + "\"");
    } else {
        if (splytapi::Config::kDebugLog) {
            Log("LINE " + to_string(line) + " ASSERT JSON STRING EQUALS SUCCESS: key:\"" + key + "\" == value:\"" + value + "\"");
        }
    }
}

void DeleteSplyt(splytapi::Splyt* splyt)
{
    delete splyt;
}

namespace tests
{
    static int successes = 0;
    static int failures = 0;

    void EntityTest()
    {
        splytapi::Splyt* splyt = InitSplyt();

        AssertJsonStringEquals(splyt->NewUser("testuser", "testContext").GetContent()["datacollector_newUser"], "description", "(Success) ", __LINE__);
        AssertJsonStringEquals(splyt->NewDevice("testdevice", "testContext").GetContent()["datacollector_newDevice"], "description", "(Success) ", __LINE__);

        AssertJsonStringEquals(splyt->NewUserChecked("testuser", "testContext").GetContent()["datacollector_newUser"], "description", "(Success) ", __LINE__);
        AssertJsonStringEquals(splyt->NewDeviceChecked("testdevice", "testContext").GetContent()["datacollector_newDevice"], "description", "(Success) ", __LINE__);

        DeleteSplyt(splyt);
    }

    void EntityStatesTest()
    {
        splytapi::Splyt* splyt = InitSplyt();

        Json::Value properties;
        properties["testprop"] = "testvalue";

        AssertJsonStringEquals(splyt->UpdateUserState("testuser", "testContext", properties).GetContent()["datacollector_updateUserState"], "description", "(Success) ", __LINE__);
        AssertJsonStringEquals(splyt->UpdateDeviceState("testdevice", "testContext", properties).GetContent()["datacollector_updateDeviceState"], "description", "(Success) ", __LINE__);

        DeleteSplyt(splyt);
    }

    void CollectionTest()
    {
        splytapi::Splyt* splyt = InitSplyt();

        AssertJsonStringEquals(splyt->UpdateCollection("testcollection", 100, -10, false, "testContext").GetContent()["datacollector_updateCollection"], "description", "(Success) ", __LINE__);

        DeleteSplyt(splyt);

    }

    void TransactionTest()
    {
        splytapi::Splyt* splyt = InitSplyt();

        AssertJsonStringEquals(splyt->transaction->Begin("testtransaction", "testcategory", 3600, "testContext").GetContent()["datacollector_beginTransaction"], "description", "(Success) ", __LINE__);
        AssertJsonStringEquals(splyt->transaction->Update("testtransaction", "testcategory", 32.23, "testContext").GetContent()["datacollector_updateTransaction"], "description", "(Success) ", __LINE__);
        AssertJsonStringEquals(splyt->transaction->End("testtransaction", "testcategory", "success", "testContext").GetContent()["datacollector_endTransaction"], "description", "(Success) ", __LINE__);

        DeleteSplyt(splyt);
    }

    void TuningTest()
    {
        splytapi::Splyt* splyt = InitSplyt();

        AssertJsonStringEquals(splyt->tuning->GetAllValues("testuser", splytapi::kEntityTypeUser).GetContent(), "testvar", "blah", __LINE__);
        AssertJsonStringEquals(splyt->tuning->GetValue("testval", "default", "testuser", splytapi::kEntityTypeUser).GetContent(), "testval", "testval", __LINE__);
        AssertJsonStringEquals(splyt->tuning->RecordValue("testval", "default", "testuser").GetContent(), "description", "(Success) ", __LINE__);

        Sleep(10000);

        //Tests default return value.
        splytapi::Config::kNetworkHost = "https://fake.error.url.com";
        AssertJsonStringEquals(splyt->tuning->GetValue("testval", "defaultval", "testuser", splytapi::kEntityTypeUser).GetContent(), "testval", "defaultval", __LINE__);
        splytapi::Config::kNetworkHost = "https://data.splyt.com";

        AssertJsonStringEquals(splyt->tuning->GetValue("testval", "default", "testuser", splytapi::kEntityTypeUser).GetContent(), "testval", "testval", __LINE__);
        AssertJsonStringEquals(splyt->tuning->GetAllValues("testuser", splytapi::kEntityTypeUser).GetContent(), "testvar", "blah", __LINE__);

        DeleteSplyt(splyt);
    }
}


void RunTest(void (*f)(), std::string name)
{
    try {
        Log("Starting test " + name + ".");
        (*f)();
        Log(name + " SUCCESS");
        tests::successes++;
    } catch (std::runtime_error e) {
        Log(name + " FAILURE: " + e.what());
        tests::failures++;
    }
}

int main ()
{
    //splytapi::Config::kDebugLog = true;
    splytapi::Config::kNetworkHost = "https://data.splyt.com";
    splytapi::Config::kTuningCacheTtl = 10000;

    Log("##### RUNNING UNIT TESTS #####");
    RunTest(tests::EntityTest, "Entity Test");
    RunTest(tests::EntityStatesTest, "Entity States Test");
    RunTest(tests::CollectionTest, "Collection Test");
    RunTest(tests::TransactionTest, "Transaction Test");
    RunTest(tests::TuningTest, "Tuning Test");

    if (tests::failures > 0) {
        Log("##### UNIT TEST FAILURE: " + to_string(tests::successes) + " TESTS PASSED - " + to_string(tests::failures) + " TESTS FAILED #####");
        return 1;
    }

    Log("##### UNIT TEST SUCCESS: " + to_string(tests::successes) + " TESTS PASSED - " + to_string(tests::failures) + " TESTS FAILED #####");
    return 0;
}