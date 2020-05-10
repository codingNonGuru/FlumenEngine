#pragma once

#include "Task.hpp"

class TaskManager
{
	friend class Engine;
	
	static Pool <Task> tasks_;

	static void Update();

public:
	static void Add(Task);
};
