#pragma once

#include "Game.h"

enum class ButtonState { BTN_IDLE = 0, BTN_HOVER, BTN_CLICK };

namespace UI
{
	class Button
	{
	private:
		GameDataRef _data;

		// button components
		sf::RectangleShape* _background;
		sf::Text* _content;

		// button colors
		sf::Color idle_color;
		sf::Color hover_color;
		sf::Color click_color;

		sf::Color text_idle;
		sf::Color text_hover;
		sf::Color text_click;

		// button state
		ButtonState _state;

	public:
		Button(GameDataRef data, sf::Vector2f pos, sf::Vector2f size,
			sf::Color idle, sf::Color hover, sf::Color click,
			sf::Color text_idle, sf::Color text_hover, sf::Color text_click,
			sf::Text content);
		~Button();

		// Getters
		bool isPressed(sf::Mouse::Button button) const;

		void update(float deltaTime);
		void render() const;

	};
}
