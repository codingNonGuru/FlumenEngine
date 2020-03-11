#include "State.hpp"
#include "StateManager.hpp"

State::State() {}

State::State(States stateType) {}

void State::Enter()
{
	HandleEnter();
}

void State::Exit()
{
	HandleExit();
}

void State::HandleEnter() {}

void State::HandleExit() {}
