#pragma once

enum class States;

class State
{
	friend class StateManager;

private:
	void Enter();

	void Exit();

protected:
	virtual void HandleEnter();

	virtual void HandleExit();

public:
	State();

	State(States);
};
