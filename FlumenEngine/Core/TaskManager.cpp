#include "TaskManager.hpp"

#define TASK_POOL_SIZE 256

Pool <Task> TaskManager::tasks_ = Pool <Task> (TASK_POOL_SIZE);

void TaskManager::Update()
{
	for(auto task = tasks_.GetStart(); task != tasks_.GetEnd(); ++task)
	{
		task->Update();
	}
}

Task* TaskManager::Add()
{
	return tasks_.Add();
}
