#pragma once

#include "FlumenCore/Conventions.hpp"
#include "FlumenCore/Delegate/Delegate.hpp"
#include "FlumenCore/Delegate/Event.hpp"

class Task
{
	friend class TaskManager;

	Event event;

	float timer_;

	bool Update();

	void Run();

public:
	Task();

	Task(float, Event);

	Task(float, void (*)());
};