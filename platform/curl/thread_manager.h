/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#ifndef SPLYT_CURL_THREAD_MANAGER_H_
#define SPLYT_CURL_THREAD_MANAGER_H_

#include <iostream>
#include <vector>
#include <cstdio>
#include <boost/thread/thread.hpp>

#include "splyt.h"

namespace splytapi
{
    class Task {
        public:
            Task(std::string nsub_path, Json::Value ncontent, std::string ncontext)
            {
                this->sub_path = nsub_path;
                this->content = ncontent;
                this->context = ncontext;
            }

            ~Task(){}

            std::string sub_path;
            Json::Value content;
            std::string context;
    };

    class ThreadManager
    {
        private:
            boost::thread* queue_thread;
            std::vector<Task*> task_queue;
            bool running;

            static void RunThread(ThreadManager* tm, std::vector<Task*>*);

        public:
            ThreadManager();
            ~ThreadManager();

            void PushTask(std::string sub_path, Json::Value content, std::string context);
            bool IsRunning();
    };
}
#endif  // SPLYT_CURL_THREAD_MANAGER_H_