#pragma once

class State
{
public:
	virtual void Init() = 0;

	virtual void HandleInput() = 0;

	virtual void Update(float deltaTime) = 0;
	virtual void Render() const = 0;

	virtual void Pause() { }
	virtual void Resume() { }

};

