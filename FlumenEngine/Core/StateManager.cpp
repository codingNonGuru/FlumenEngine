#include "StateManager.hpp"
#include "State.hpp"

#define MAXIMUM_STATE_COUNT 16

States StateManager::currentState_ = States();

States StateManager::nextState_ = States();

States StateManager::previousState_ = States();

Map <State*, States> StateManager::states_ = Map <State*, States> (MAXIMUM_STATE_COUNT);

void StateManager::Enter(States state)
{
	if(currentState_ == state)
		return;

	previousState_ = currentState_;

	currentState_ = state;

	nextState_ = state;

	auto previousState = states_.Get(previousState_);
	if(previousState != nullptr && *previousState != nullptr)
	{
		(*previousState)->Exit();
	}

	auto nextState = states_.Get(nextState_);
	if(nextState != nullptr && *nextState != nullptr)
	{
		(*nextState)->Enter();
	}
}

void StateManager::Register(State* state, States stateType)
{
	auto statePointer = states_.Get(stateType);
	if(statePointer != nullptr)
		return;

	statePointer = states_.Add(stateType);
	if(statePointer == nullptr)
		return;

	*statePointer = state;
}

States StateManager::GetNextState()
{
	return nextState_;
}

States StateManager::GetPreviousState()
{
	return previousState_;
}
