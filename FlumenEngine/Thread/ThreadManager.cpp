#include "ThreadManager.h"

using namespace engine;

static constexpr auto TASK_WAIT_DURATION = 1;

ThreadManager::ThreadManager() 
{
    threads.Initialize(THREAD_COUNT);

    asyncTasks.Initialize(MAXIMUM_ASYNC_TASK_COUNT);

    finishedTasks.Initialize(MAXIMUM_ASYNC_TASK_COUNT);
}

void ThreadManager::AwaitThreadFinish()
{
    for(auto &thread : threads)
    {
        thread.join();
    }

    threads.Reset();
}

void ThreadManager::Update()
{
    finishedTasks.Reset();

    for(auto &task : asyncTasks)
    {
        std::future_status status = task.Future.wait_for(std::chrono::microseconds(TASK_WAIT_DURATION));
        switch(status)
        {
        case std::future_status::ready:
            *finishedTasks.Add() = &task;
            break;
        case std::future_status::timeout:
            break;
        }
    }

    for(auto &task : finishedTasks)
    {
        task->OnFinish.Invoke();

        asyncTasks.RemoveAt(task);
    }
}

