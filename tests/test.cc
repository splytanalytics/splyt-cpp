/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#include <string>

#include "splyt.h"
#include "util/log.h"

template <typename T>
std::string to_string(T value)
{
  std::ostringstream os ;
  os << value ;
  return os.str() ;
}

std::string random_string(const int len) {
    char s[len];
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
    return std::string(s);
}

void Log(std::string s)
{
    std::cout << "TEST LOG: " + s << std::endl;
}

//Test functions.
splytapi::Splyt* InitSplyt()
{
    //Initialize the Splyt API.
    splytapi::Splyt* splyt = splytapi::Init("emr-test-test", "testuser" + random_string(8), "testdevice" + random_string(8), "testContext");
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

    static splytapi::Splyt* splyt;

    void EntityTest()
    {

        AssertJsonStringEquals(splyt->NewUser("testuser" + random_string(8), "testContext").GetContent()["datacollector_newUser"], "description", "(Success) ", __LINE__);
        AssertJsonStringEquals(splyt->NewDevice("testdevice" + random_string(8), "testContext").GetContent()["datacollector_newDevice"], "description", "(Success) ", __LINE__);

        AssertJsonStringEquals(splyt->NewUserChecked("testuser" + random_string(8), "testContext").GetContent()["datacollector_newUser"], "description", "(Success) ", __LINE__);
        AssertJsonStringEquals(splyt->NewDeviceChecked("testdevice" + random_string(8), "testContext").GetContent()["datacollector_newDevice"], "description", "(Success) ", __LINE__);

        //DeleteSplyt(splyt);
    }

    void EntityStatesTest()
    {
        //splytapi::Splyt* splyt = InitSplyt();

        Json::Value properties;
        properties["testprop"] = "testvalue";
        properties["random_number"] = rand() % 1000;
        properties["health"] = rand() % 100;

        AssertJsonStringEquals(splyt->UpdateUserState("testuser" + random_string(8), "testContext", properties).GetContent()["datacollector_updateUserState"], "description", "(Success) ", __LINE__);
        AssertJsonStringEquals(splyt->UpdateDeviceState("testdevice" + random_string(8), "testContext", properties).GetContent()["datacollector_updateDeviceState"], "description", "(Success) ", __LINE__);

        //DeleteSplyt(splyt);
    }

    void CollectionTest()
    {
        //splytapi::Splyt* splyt = InitSplyt();

        AssertJsonStringEquals(splyt->UpdateCollection("testcollection" + random_string(8), 100, -((rand() % 50) + 5), false, "testContext").GetContent()["datacollector_updateCollection"], "description", "(Success) ", __LINE__);
        AssertJsonStringEquals(splyt->RecordPurchase("testpurchase" + random_string(8), (rand() % 50) + 5, "USD", "success", "offer1", "pointofsale1", "Cool Gear", "testContext").GetContent()["datacollector_endTransaction"], "description", "(Success) ", __LINE__);

        //DeleteSplyt(splyt);
    }

    void TransactionTest()
    {
        //splytapi::Splyt* splyt = InitSplyt();

        std::string trans_id = "testtransaction" + random_string(8);
        std::string trans_cat = "testcategory" + random_string(8);
        AssertJsonStringEquals(splyt->transaction->Begin(trans_id, trans_cat, 3600, "testContext").GetContent()["datacollector_beginTransaction"], "description", "(Success) ", __LINE__);
        AssertJsonStringEquals(splyt->transaction->Update(trans_id, trans_cat, (rand() % 90) + 10, "testContext").GetContent()["datacollector_updateTransaction"], "description", "(Success) ", __LINE__);
        AssertJsonStringEquals(splyt->transaction->End(trans_id, trans_cat, "success", "testContext").GetContent()["datacollector_endTransaction"], "description", "(Success) ", __LINE__);

        //DeleteSplyt(splyt);
    }
}


void RunTest(void (*f)(), std::string name)
{
    async_test_error = false;
    async_test_error_message = "";

    try {
        tests::splyt = InitSplyt();
        //Log("Starting test " + name + ".");
        (*f)();
        if (async_test_error) {
            throw std::runtime_error(async_test_error_message);
        }
        //Log(name + " SUCCESS");
        tests::successes++;


    } catch (std::runtime_error e) {
        Log(name + " FAILURE: " + e.what());
        tests::failures++;
    }

    DeleteSplyt(tests::splyt);
}

int main()
{
    //splytapi::Config::kDebugLog = true;
    //splytapi::Config::kNetworkEnableThreading = true;
    splytapi::Config::kNetworkHost = "https://splyt-test9-data.splyt.com";
    splytapi::Config::kTuningCacheTtl = 10000; //Tuning variable cache TTL set to 10 seconds.
    splytapi::Config::kNetworkTimeout = 50;

    Log("##### RUNNING UNIT TESTS #####");

    int total_tests = 10000;
    int count = 0;
    while (count < total_tests) {
        RunTest(tests::EntityTest, "Entity Test");

        RunTest(tests::EntityStatesTest, "Entity States Test");

        RunTest(tests::CollectionTest, "Collection Test");

        RunTest(tests::TransactionTest, "Transaction Test");
        count++;

        Log("Progress: " + to_string(((float)count/(float)total_tests)*100.0f) + "%");
    }

    std::string info = to_string(tests::successes) + " TESTS PASSED - " + to_string(tests::failures) + " TESTS FAILED #####";
    std::cout << std::endl;
    Log("###### STATISTICS ######");
    Log("Total Sent Datapoints: " + to_string(splytapi::Network::stat_total_datapoints));
    for(std::map<std::string, int>::iterator it = splytapi::Network::stat_datapoint_list.begin(); it != splytapi::Network::stat_datapoint_list.end(); it++) {
        Log(it->first + ": " + to_string(it->second));
    }
    std::cout << std::endl;

    if (tests::failures > 0) {
        Log("##### UNIT TEST FAILURE: " + info);
        return 1;
    }

    Log("##### UNIT TEST SUCCESS: " + info);
    return 0;
}