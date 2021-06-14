#pragma once

#include "../UIElement.h"
#include "../Button.h"

namespace UI
{
	class MessageBox :
		public UIElement
	{
	private:
		std::map<std::string, sf::Text*> _texts; // texts
		std::map<std::string, UI::Button*> _buttons; // buttons
		UI::Button* _closeButton; // close button (separated from others)

		sf::RectangleShape* _dragZone; // upper drag zone

		bool _isActive; // is box active

	public:
		MessageBox(GameDataRef data, std::string title, std::string info, sf::Vector2f position);
		~MessageBox();

		bool isActive() const;

		void setPosition(sf::Vector2f position) override;

		// basic functions
		void update(float deltaTime) override;
		void render() const override;

	};
}
