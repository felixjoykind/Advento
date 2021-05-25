#pragma once

#include "UIElement.h"

namespace UI
{
	class Textbox :
		public UIElement
	{
	private:
		std::string _content;
		sf::Text* _text;

	public:
		Textbox(GameDataRef data, sf::Vector2f pos, sf::Vector2f size);
		~Textbox();

		void handleInput();

		void update(float deltaTime) override;
		void render() const override;

	};
}
