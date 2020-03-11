#pragma once

#include "Task.hpp"

class TaskManager
{
	static Pool <Task> tasks_;

public:
	static void Update();

	static Task* Add();
};
