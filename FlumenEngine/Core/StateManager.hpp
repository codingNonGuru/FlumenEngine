#pragma once

#include "FlumenEngine/Core/Types.hpp"
#include "FlumenCore/Conventions.hpp"

class State;

class StateManager
{
	friend class State;

	State * currentState;

	State * nextState;

	State * previousState;

	Array <State *> states;

	StateManager();

	static StateManager instance;

	static void Enter(State *);

	static void Register(State *);

public:
	static State* GetNextState();

	static State* GetPreviousState();
};
