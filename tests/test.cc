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
    splyt::Log::Info("Starting tests.");

    splyt::Init("knetik-bubblepop-test", "testuser", "", "testContext");

    //splyt::NewUserChecked("testuser");

    splyt::BeginTransaction("testuser,", "", "test_cat", 3600, "", "testContext");

    //splyt::Tuning::RecordValue("testvar", "testuser", "", "newtestval");

    //splyt::Tuning::Refresh("testvar", "testuser");

    Sleep(2000);

    splyt::Tuning::GetValue("testvar", "testuser", splyt::kEntityTypeUser);

    splyt::Tuning::GetAllValues("testuser", splyt::kEntityTypeUser);

    //splyt::UpdateTransaction("testuer", "", "test_cat", 24, "", "testContext");

    //splyt::EndTransaction("testuser", "", "test_cat", "success", "", "testContext");

    return 0;
}