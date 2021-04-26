#pragma once

#include <stack>
#include <memory>

#include "states/State.h"

typedef std::unique_ptr<State> StateRef;

class StateMachine
{
private:
	std::stack<StateRef> _states;

	StateRef _newState;

	bool _isAdding;
	bool _isReplacing;
	bool _isRemoving;

public:
	StateMachine();
	~StateMachine();

	void AddState(StateRef newState, bool isReplacing);
	void RemoveState();
	StateRef& GetActiveState();
	void ProcessStateChanges();

};

