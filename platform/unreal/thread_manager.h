/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#ifndef SPLYT_CURL_THREAD_MANAGER_H_
#define SPLYT_CURL_THREAD_MANAGER_H_

#include "EngineMinimal.h"
#include "Http.h"
#include "splyt.h"

namespace splytapi
{
    class Network;

    class ThreadManager
    {
        private:
            Network* network;

        public:
            ThreadManager(Network* n);
            ~ThreadManager(){}

            void PushTask(NetworkCallback callback, std::string sub_path, Json::Value content, std::string context);
    };
}
#endif  // SPLYT_CURL_THREAD_MANAGER_H_