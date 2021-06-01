#pragma once

#include "engine/ui/IClickable.h"
#include "engine/ui/IHoverable.h"

namespace UI
{
	class Button
		: public UIElement, public IHoverable, public IClickable
	{
	private:
		// button components
		sf::Text* _content;

		// button colors
		sf::Color idle_color;
		sf::Color hover_color;
		sf::Color click_color;

		sf::Color text_idle;
		sf::Color text_hover;
		sf::Color text_click;

	public:
		Button(GameDataRef data, sf::Vector2f pos, sf::Vector2f size,
			sf::Color idle, sf::Color hover, sf::Color click,
			sf::Color text_idle, sf::Color text_hover, sf::Color text_click,
			sf::Text content);
		~Button();

		// Getters
		bool isPressed(sf::Mouse::Button button);

		void update(float deltaTime) override;
		void render() const override;

	};
}
