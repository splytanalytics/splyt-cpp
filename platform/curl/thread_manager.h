/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#ifndef SPLYT_CURL_THREAD_MANAGER_H_
#define SPLYT_CURL_THREAD_MANAGER_H_

#include <iostream>
#include <vector>
#include <cstdio>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include "splyt.h"

namespace splytapi
{
    class Network;

    class Task {
        public:
            Task(NetworkCallback ncallback, std::string nsub_path, Json::Value ncontent, std::string ncontext)
            {
                this->callback = ncallback;
                this->sub_path = nsub_path;
                this->content = ncontent;
                this->context = ncontext;
            }

            ~Task(){}

            NetworkCallback callback;
            std::string sub_path;
            Json::Value content;
            std::string context;
    };

    class ThreadManager
    {
        private:
            Network* network;
            boost::thread* queue_thread;
            std::vector<Task*> task_queue;
            boost::mutex mtx;
            bool running;

            static void RunThread(ThreadManager* tm, std::vector<Task*>*);

        public:
            ThreadManager(Network* n);
            ~ThreadManager();

            void PushTask(NetworkCallback callback, std::string sub_path, Json::Value content, std::string context);
            bool IsRunning();
            Network* GetNetwork();
    };
}
#endif  // SPLYT_CURL_THREAD_MANAGER_H_