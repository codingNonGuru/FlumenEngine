#include "TaskManager.hpp"

#define TASK_POOL_SIZE 256

Pool <Task> TaskManager::tasks_ = Pool <Task> (TASK_POOL_SIZE);

Array <Task *> recycleBin = Array <Task *> (TASK_POOL_SIZE);

void TaskManager::Update()
{
	recycleBin.Reset();
	//std::cout<<"task count: "<<tasks_.GetSize()<<"\n";

	for(auto task = tasks_.GetStart(); task != tasks_.GetEnd(); ++task)
	{
		bool hasExecuted = task->Update();
		if(hasExecuted)
		{
			*recycleBin.Allocate() = task;
		}
	}

	for(auto task = recycleBin.GetStart(); task != recycleBin.GetEnd(); ++task)
	{
		tasks_.Remove(*task);
	}
}

Task* TaskManager::Add()
{
	return tasks_.Add();
}
