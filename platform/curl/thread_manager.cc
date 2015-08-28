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
        //std::cout << "THREAD " << task_queue << ": RUNNING" << std::endl;

        while (tm->IsRunning() || (task_queue->size() > 0 && Config::kNetworkEnableGracefulShutdown)) {
            if (task_queue->size() > 0) {
                Task* task = (*task_queue)[0];
                task_queue->erase(task_queue->begin());

                //std::cout << "THREAD " << task_queue << ": POP TASK - " << task->sub_path << std::endl;
                try {
                    SplytResponse response = tm->GetNetwork()->Call(task->sub_path, task->content, task->context);
                    //std::cout << "THREAD " << task_queue << ": TASK SUCCESSFUL - " << task << std::endl;

                    if (task->callback != NULL) {
                        task->callback(response);
                    }
                } catch (splytapi::splyt_exception e) {
                    splytapi::SplytResponse resp = e.GetResponse();

                    //std::cout << "THREAD " << task_queue << ": TASK FAILED - " << task << ": " + resp.GetErrorMessage() << std::endl;

                    if (task->callback != NULL) {
                        task->callback(resp);
                    }
                }

                delete task;
            }
        }

        //std::cout << "THREAD " << task_queue << ": STOPPED - TASKS REMAINING: " << task_queue->size() << std::endl;
    }

    void ThreadManager::PushTask(NetworkCallback callback, std::string sub_path, Json::Value content, std::string context)
    {
        if (!Config::kNetworkEnableThreading) {
            splytapi::ThrowDummyResponseException("splytapi::Config::kNetworkEnableThreading must be set to true before calling splyt::Init() to enable the use of asynchronous calls.");
        }
        //std::cout << "MAIN " << &task_queue << ": PUSH TASK" << std::endl;

        Task* task = new Task(callback, sub_path, content, context);
        mtx.lock();
        task_queue.push_back(task);
        mtx.unlock();
    }

    bool ThreadManager::IsRunning()
    {
        return this->running;
    }

    Network* ThreadManager::GetNetwork()
    {
        return this->network;
    }
}