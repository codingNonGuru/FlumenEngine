#pragma once

#include "FlumenCore/Conventions.hpp"
#include "FlumenCore/Delegate/Delegate.hpp"

class Task
{
	friend class TaskManager;

	bool isValid_;

	Delegate event_;

	float timer_;

	bool Update();

	void Run();

public:
	Task();

	template <class ClassType, class ObjectType>
	void Initialize(ObjectType*, void (ClassType::*)(), float);
};

template <class ClassType, class ObjectType>
void Task::Initialize(ObjectType *object, void (ClassType::*function)(), float timer)
{
	event_.Add(object, function);

	timer_ = timer;

	isValid_ = true;
}
