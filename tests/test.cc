#include <stdio.h>
#include <string>

#include "splyt.h"
#include "util/log.h"

namespace test
{
    void SplytCallback(splyt::SplytResponse response)
    {
        splyt::Log::Info("Callback test: " + response.GetContent().toStyledString());
    }
}

int main ()
{
    splyt::Log::Info("Starting tests.");

    splyt::Init("knetik-bubblepop-test", "testuser", "");

    //splyt::NewUserChecked("testuser", test::SplytCallback);

    splyt::Tuning::GetTuningVal("testvar", "testid", splyt::kEntityTypeUser);

    Json::Value properties;
    //splyt::BeginTransaction("testuser,", "", "test_cat", 3600, "", properties);

    //splyt::UpdateTransaction("testuer", "", "test_cat", 24, "", properties);

    //splyt::EndTransaction("testuser", "", "test_cat", "success", "", properties);

    return 0;
}