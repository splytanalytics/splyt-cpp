/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#include "thread_manager.h"

namespace splytapi
{
    ThreadManager::ThreadManager(Network* n)
    {
        this->network = n;
    }

    void DummyCallback(splytapi::SplytResponse response)
    {
        //Dummy callback does nothing, just used to signifiy an asynchronous call that wasn't already passed a callback.
    }

    void ThreadManager::PushTask(NetworkCallback callback, std::string sub_path, Json::Value content, std::string context)
    {
        if (callback == NULL) {
            callback = &DummyCallback;
        }

        //Threading is already handled by Unreal Engine, so we simply pass data along to the network class.
        this->network->Call(sub_path, content, context, callback);
    }
}