#include "StateManager.hpp"
#include "State.hpp"

#define MAXIMUM_STATE_COUNT 16

StateManager StateManager::instance = StateManager();

StateManager::StateManager()
{
	currentState = nullptr;

	nextState = nullptr;

	previousState = nullptr;

	states = Array <State *> (MAXIMUM_STATE_COUNT);
}

void StateManager::Enter(State * state)
{
	if(instance.currentState == state)
		return;

	if(instance.currentState != nullptr)
	{
		instance.currentState->HandleExit();
	}

	//instance.previousState = instance.currentState;

	instance.currentState = state;

	instance.currentState->HandleEnter();

	//instance.nextState = state;

	//instance.nextState->HandleEnter();
}

void StateManager::Register(State* state)
{
	*instance.states.Allocate() = state;
}

State* StateManager::GetNextState()
{
	return instance.nextState;
}

State* StateManager::GetPreviousState()
{
	return instance.previousState;
}

void StateManager::Update()
{
	for(auto state : instance.states)
	{
		state->Update();
	}
}