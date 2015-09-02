/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#include <string>

#include "splyt.h"
#include "util/log.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>

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
#endif

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

bool async_test_error = false;
std::string async_test_error_message = "";
void TestCallback(splytapi::SplytResponse response)
{
    if (!response.IsSuccessful()) {
        if (!async_test_error) {
            async_test_error_message = response.GetErrorMessage();
            async_test_error = true;
        }
    }
}

namespace tests
{
    static int successes = 0;
    static int failures = 0;

    void InitTest()
    {
        splytapi::Splyt* splyt = InitSplyt();

        AssertJsonStringEquals(splyt->tuning->GetValue("testval", "default", "testuser", splytapi::kEntityTypeUser).GetContent(), "testval", "testval", __LINE__);

        DeleteSplyt(splyt);
    }

    void EntityTest()
    {
        splytapi::Splyt* splyt = InitSplyt();

        AssertJsonStringEquals(splyt->NewUser("testuser", "testContext").GetContent()["datacollector_newUser"], "description", "(Success) ", __LINE__);
        AssertJsonStringEquals(splyt->NewDevice("testdevice", "testContext").GetContent()["datacollector_newDevice"], "description", "(Success) ", __LINE__);

        AssertJsonStringEquals(splyt->NewUserChecked("testuser", "testContext").GetContent()["datacollector_newUser"], "description", "(Success) ", __LINE__);
        AssertJsonStringEquals(splyt->NewDeviceChecked("testdevice", "testContext").GetContent()["datacollector_newDevice"], "description", "(Success) ", __LINE__);

        DeleteSplyt(splyt);
    }
    void EntityAsyncTest()
    {
        splytapi::Splyt* splyt = InitSplyt();

        splyt->NewUserAsync(&TestCallback, "testuser", "testContext");
        splyt->NewDeviceAsync(&TestCallback, "testdevice", "testContext");

        splyt->NewUserCheckedAsync(&TestCallback, "testuser", "testContext");
        splyt->NewDeviceCheckedAsync(&TestCallback, "testdevice", "testContext");

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
    void EntityStatesAsyncTest()
    {
        splytapi::Splyt* splyt = InitSplyt();

        Json::Value properties;
        properties["testprop"] = "testvalue";

        splyt->UpdateUserStateAsync(&TestCallback, "testuser", "testContext", properties);
        splyt->UpdateDeviceStateAsync(&TestCallback, "testdevice", "testContext", properties);

        DeleteSplyt(splyt);
    }

    void CollectionTest()
    {
        splytapi::Splyt* splyt = InitSplyt();

        AssertJsonStringEquals(splyt->UpdateCollection("testcollection", 100, -10, false, "testContext").GetContent()["datacollector_updateCollection"], "description", "(Success) ", __LINE__);
        AssertJsonStringEquals(splyt->RecordPurchase("testpurchase", 19.99, "USD", "success", "offer1", "pointofsale1", "Cool Gear", "testContext").GetContent()["datacollector_endTransaction"], "description", "(Success) ", __LINE__);

        DeleteSplyt(splyt);
    }
    void CollectionAsyncTest()
    {
        splytapi::Splyt* splyt = InitSplyt();

        splyt->UpdateCollectionAsync(&TestCallback, "testcollection", 100, -10, false, "testContext");
        splyt->RecordPurchaseAsync(&TestCallback, "testpurchase", 19.99, "USD", "success", "offer1", "pointofsale1", "Cool Gear", "testContext");

        DeleteSplyt(splyt);
    }

    void TransactionTest()
    {
        splytapi::Splyt* splyt = InitSplyt();

        AssertJsonStringEquals(splyt->transaction->Begin("testtransaction", "testcategory", 3600, "testContext").GetContent()["datacollector_beginTransaction"], "description", "(Success) ", __LINE__);
        AssertJsonStringEquals(splyt->transaction->Update("testtransaction", "testcategory", 32.23, "testContext").GetContent()["datacollector_updateTransaction"], "description", "(Success) ", __LINE__);
        AssertJsonStringEquals(splyt->transaction->End("testtransaction", "testcategory", "success", "testContext").GetContent()["datacollector_endTransaction"], "description", "(Success) ", __LINE__);
        AssertJsonStringEquals(splyt->transaction->BeginEnd("begin-end_testtransaction", "begin-end_testcategory", "success", "testContext").GetContent()["datacollector_endTransaction"], "description", "(Success) ", __LINE__);

        DeleteSplyt(splyt);
    }
    void TransactionAsyncTest()
    {
        splytapi::Splyt* splyt = InitSplyt();

        splyt->transaction->BeginAsync(&TestCallback, "testtransaction", "testcategory", 3600, "testContext");
        splyt->transaction->UpdateAsync(&TestCallback, "testtransaction", "testcategory", 32.23, "testContext");
        splyt->transaction->EndAsync(&TestCallback, "testtransaction", "testcategory", "success", "testContext");
        splyt->transaction->BeginEndAsync(&TestCallback, "begin-end_testtransaction", "begin-end_testcategory", "success", "testContext");

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
    void TuningAsyncTest()
    {
        splytapi::Splyt* splyt = InitSplyt();

        splyt->tuning->RecordValueAsync(&TestCallback, "testval", "default", "testuser");

        DeleteSplyt(splyt);
    }
}


void RunTest(void (*f)(), std::string name)
{
    async_test_error = false;
    async_test_error_message = "";

    try {
        Log("Starting test " + name + ".");
        (*f)();
        if (async_test_error) {
            throw std::runtime_error(async_test_error_message);
        }
        Log(name + " SUCCESS");
        tests::successes++;
    } catch (std::runtime_error e) {
        Log(name + " FAILURE: " + e.what());
        tests::failures++;
    }
}

int main()
{
    //splytapi::Config::kDebugLog = true;
    splytapi::Config::kNetworkEnableThreading = true;
    splytapi::Config::kNetworkHost = "https://data.splyt.com";
    splytapi::Config::kTuningCacheTtl = 10000; //Tuning variable cache TTL set to 10 seconds.

    Log("##### RUNNING UNIT TESTS #####");

    RunTest(tests::InitTest, "Init Test");

    RunTest(tests::EntityTest, "Entity Test");
    RunTest(tests::EntityAsyncTest, "Entity Async Test");

    RunTest(tests::EntityStatesTest, "Entity States Test");
    RunTest(tests::EntityStatesAsyncTest, "Entity States Async Test");

    RunTest(tests::CollectionTest, "Collection Test");
    RunTest(tests::CollectionAsyncTest, "Collection Async Test");

    RunTest(tests::TransactionTest, "Transaction Test");
    RunTest(tests::TransactionAsyncTest, "Transaction Async Test");

    RunTest(tests::TuningTest, "Tuning Test");
    RunTest(tests::TuningAsyncTest, "Tuning Async Test");

    std::string info = to_string(tests::successes) + " TESTS PASSED - " + to_string(tests::failures) + " TESTS FAILED #####";
    if (tests::failures > 0) {
        Log("##### UNIT TEST FAILURE: " + info);
        return 1;
    }

    Log("##### UNIT TEST SUCCESS: " + info);
    return 0;
}