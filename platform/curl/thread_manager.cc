/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#include "thread_manager.h"

namespace splytapi
{
    ThreadManager::ThreadManager(Network* n)
    {
        if (Config::kNetworkEnableThreading) {
            this->network = n;
            Log::Info("Thread manager init.");

            running = true;
            queue_thread = new boost::thread(&ThreadManager::RunThread, this, &task_queue);
        }
    }

    ThreadManager::~ThreadManager()
    {
        splytapi::Log::Info("Stopping and joining thread.");
        running = false;
        queue_thread->join();
        splytapi::Log::Info("Freeing thread memory.");
        delete queue_thread;
        queue_thread = NULL;
    }

    void ThreadManager::RunThread(ThreadManager* tm, std::vector<Task*>* task_queue)
    {
        while (tm->IsRunning() || (task_queue->size() > 0 && Config::kNetworkEnableGracefulShutdown)) {
            if (task_queue->size() > 0) {
                tm->GetMutex()->lock();
                Task* task = (*task_queue)[0];
                task_queue->erase(task_queue->begin());
                tm->GetMutex()->unlock();

                try {
                    SplytResponse response = tm->GetNetwork()->Call(task->sub_path, task->content, task->context);

                    if (task->callback != NULL) {
                        task->callback(response);
                    }
                } catch (splytapi::splyt_exception e) {
                    splytapi::SplytResponse resp = e.GetResponse();

                    if (task->callback != NULL) {
                        task->callback(resp);
                    }
                }

                delete task;
            }
        }
    }

    void ThreadManager::PushTask(NetworkCallback callback, std::string sub_path, Json::Value content, std::string context)
    {
        if (!Config::kNetworkEnableThreading) {
            splytapi::ThrowDummyResponseException("splytapi::Config::kNetworkEnableThreading must be set to true before calling splyt::Init() to enable the use of asynchronous calls.");
        }

        Task* task = new Task(callback, sub_path, content, context);
        mtx.lock();
        task_queue.push_back(task);
        mtx.unlock();
    }

    bool ThreadManager::IsRunning()
    {
        return this->running;
    }

    boost::mutex* ThreadManager::GetMutex()
    {
        return &this->mtx;
    }

    Network* ThreadManager::GetNetwork()
    {
        return this->network;
    }
}