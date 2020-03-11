#include "Task.hpp"
#include "FlumenCore/Time.hpp"

Task::Task()
{
	isValid_ = false;

	timer_ = 0.0f;
}

void Task::Update()
{
	if(isValid_ == false)
		return;

	timer_ -= Time::GetDelta();

	if(timer_ < 0.0f)
	{
		Run();
	}
}

void Task::Run()
{
	isValid_ = false;

	timer_ = 0.0f;

	event_.Invoke();

	event_.Clear();
}
