#include "Task.hpp"
#include "FlumenCore/Time.hpp"

Task::Task()
{
	timer_ = 0.0f;
}

Task::Task(float timer, void (*function)())
{
	event = Event(function);

	timer_ = timer;
}

Task::Task(float timer, Event _event)
{
	event = _event;
	
	timer_ = timer;
}

bool Task::Update()
{
	timer_ -= Time::GetDelta();

	if(timer_ < 0.0f)
	{
		Run();
		return true;
	}

	return false;
}

void Task::Run()
{
	event.Invoke();
}
