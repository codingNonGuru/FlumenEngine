#pragma once

enum class States;

class State
{
	friend class StateManager;

protected:
	virtual void HandleEnter() = 0;

	virtual void HandleExit() = 0;

	State();

public:
	void Enter();
};
