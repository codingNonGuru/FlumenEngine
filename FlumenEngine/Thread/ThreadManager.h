#pragma once

#include <thread>
#include <atomic>
#include <iostream>
#include <future>
#include <chrono>

#include "FlumenCore/Singleton.h"

namespace engine
{
    class ThreadManager : public core::Singleton <ThreadManager>
    {
        std::thread activeThread;

        std::future <int> task;

        std::atomic<bool> isDone {false};

        bool isInitialized {false};

        bool isWorking {false};

    public:
        ThreadManager() 
        {
            activeThread = std::thread([] {});
        }
        /*template<typename Callable, typename... Arguments>
        void LaunchThread(Callable function, Arguments... arguments)
        {
            if(isInitialized == false)
            {
                isInitialized = true;
            }
            else
            {
                if(isDone == false)
                {
                    return;
                }
                
                activeThread.join();
            }

            isDone = false;

            //activeThread = std::thread(function, arguments...);

            activeThread = std::thread
            (
                [this, function, arguments...] 
                {
                    arguments[0]->function(arguments...);
                    
                    this->isDone = true;
                }
            );
        }*/

        template<typename Callable, typename... Arguments>
        void LaunchThread(Callable function, Arguments... arguments)
        {
            /*std::cout<<"threads "<<activeThread.hardware_concurrency()<<"\n";
            auto flag = activeThread.joinable();
            std::cout<<"flag: "<<int(flag)<<'\n';
            if(flag == true)
            {
                activeThread.join();

                activeThread = std::thread(function, arguments...);
            }
            else if(isInitialized == false)
            {
                isInitialized = true;

                activeThread = std::thread(function, arguments...);
            }*/

            /*if(activeThread.joinable())
            {
                std::cout<<"cucu\n";
                activeThread.join();
                std::cout<<"cucu2\n";

                activeThread = std::thread(function, arguments...);
            }*/

            using namespace std::chrono_literals;

            if(isInitialized == false)
            {
                isInitialized = true;
                task = std::async(std::launch::async, function, arguments...);
                return;
            }

            auto status = task.wait_for(0ms);
            if(status == std::future_status::ready)
            {
                std::cout<<"done\n";
                task = std::async(std::launch::async, function, arguments...);
            }
            else
            {
                std::cout<<"not done\n";
            }
            //std::cout<<task.get()<<"\n";

            //task.get();
        }
    };
}