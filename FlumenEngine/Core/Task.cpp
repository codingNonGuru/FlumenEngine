#include "Task.hpp"
#include "FlumenCore/Time.hpp"

Task::Task()
{
	isValid_ = false;

	timer_ = 0.0f;
}

bool Task::Update()
{
	if(isValid_ == false)
		return false;

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
	//std::cout<<"RUUUUN\n";
	isValid_ = false;

	timer_ = 0.0f;

	event_.Invoke();

	event_.Clear();
}
