#include "TaskManager.hpp"

#define TASK_POOL_SIZE 256

Pool <Task> TaskManager::tasks_ = Pool <Task> (TASK_POOL_SIZE);

Array <Task *> recycleBin = Array <Task *> (TASK_POOL_SIZE);

void TaskManager::Update()
{
	recycleBin.Reset();

	tasks_.Do([](auto &task) -> bool 
	{
		bool hasExecuted = task.Update();
		if(hasExecuted)
		{
			*recycleBin.Allocate() = &task;
		}
	});

	for(auto task = recycleBin.GetStart(); task != recycleBin.GetEnd(); ++task)
	{
		tasks_.Remove(*task);
	}
}

void TaskManager::Add(Task task)
{
	*tasks_.Add() = task;
}
