#pragma once

#include "states/SavesState.h"
#include "engine/ui/Button.h"

class MainMenuState
	: public State
{
private:
	GameDataRef _data;

	std::map<std::string, UI::Button*> _buttons;

public:
	MainMenuState(GameDataRef data);
	~MainMenuState();

	void Init() override;

	void HandleInput() override;

	void Update(float deltaTime) override;
	void Render() const override;

};

