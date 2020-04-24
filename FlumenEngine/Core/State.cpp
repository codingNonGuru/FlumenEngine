#include "State.hpp"
#include "StateManager.hpp"

State::State() 
{
	StateManager::Register(this);
}

void State::Enter()
{
	StateManager::Enter(this);
}


