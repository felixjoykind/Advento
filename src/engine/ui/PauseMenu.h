#pragma once

#include "elements/Button.h"

class PauseMenu
{
private:
	// data
	GameDataRef _data;
	bool _active;

	// ui
	sf::Text* _title;
	std::map<std::string, UI::Button*> _buttons;
	sf::RectangleShape _background;
	sf::RectangleShape _panel;

public:
	PauseMenu(GameDataRef data);
	~PauseMenu();

	// getter
	bool isActive() const;

	// setters
	void setActive(bool value);

	// basic functions
	void update(float deltaTime);
	void render() const;

};

