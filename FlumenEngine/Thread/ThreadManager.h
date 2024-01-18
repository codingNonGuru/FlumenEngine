#pragma once

#include <thread>
#include <atomic>
#include <iostream>
#include <future>
#include <chrono>

#include "FlumenCore/Singleton.h"
#include "FlumenCore/Container/Pool.hpp"
#include "FlumenCore/Container/Array.hpp"
#include "FlumenCore/Delegate/Event.hpp"

#include "FlumenEngine/Config.h"
#include "FlumenEngine/Thread/ThreadResultData.h"

namespace engine
{
    class ThreadManager : public core::Singleton <ThreadManager>
    {
        friend class core::Singleton <ThreadManager>;

        struct TaskData
        {
            std::future <int> Future;

            Event OnFinish;
        };

        static constexpr auto THREAD_OVERFLOW_MESSAGE = "Synchronous thread pool is overflowing.\n";

        static constexpr auto ASYNC_TASK_OVERFLOW_MESSAGE = "Asynchronous task pool is overflowing.\n";

        container::Pool <std::thread> threads;

        container::Pool <TaskData> asyncTasks;

        container::Array <TaskData *> finishedTasks;

        ThreadManager();

    public:
        template<typename Callable, typename InputType, typename ResultType>
        void LaunchSyncThread(Callable function, InputType &inputData, ThreadResultData <ResultType> &resultData)
        {
            assert((threads.GetSize() < THREAD_COUNT) && THREAD_OVERFLOW_MESSAGE);

            auto threadPointer = threads.Add(); 

            auto threadIndex = threads.GetIndex(threadPointer);

            auto processedResultData = resultData.GetResult(threadIndex);
            
            *threadPointer = std::thread(function, inputData, processedResultData);
        }

        template<typename Callable, typename... Arguments>
        void LaunchAsyncThread(Event event, Callable function, Arguments... arguments)
        {
            assert((asyncTasks.GetSize() < MAXIMUM_ASYNC_TASK_COUNT) && ASYNC_TASK_OVERFLOW_MESSAGE);

            *asyncTasks.Add() = {std::async(std::launch::async, function, arguments...), event};
        }

        void AwaitThreadFinish();

        void Update();
    };
}