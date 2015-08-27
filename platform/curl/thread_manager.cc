/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#include "thread_manager.h"

namespace splytapi
{
    ThreadManager::ThreadManager()
    {
        Log::Info("Thread manager init.");

        running = true;
        queue_thread = new boost::thread(&ThreadManager::RunThread, this, &task_queue);
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
        std::cout << "THREAD: RUNNING" << std::endl;

        while (tm->IsRunning()) {
            if (task_queue->size() > 0) {
                Task* task = (*task_queue)[0];
                task_queue->erase(task_queue->begin());
                std::cout << "THREAD: POP TASK - " << task << std::endl;
                delete task;
            }
        }

        std::cout << "THREAD: STOPPED"<< std::endl;
    }

    void ThreadManager::PushTask(std::string sub_path, Json::Value content, std::string context)
    {
        Task* task = new Task(sub_path, content, context);
        Log::Info("MAIN: PUSH TASK");
        task_queue.push_back(task);
    }

    bool ThreadManager::IsRunning()
    {
        return this->running;
    }
}