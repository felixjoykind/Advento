#pragma once

#include "./engine/ui/IDragable.h"
#include "../Button.h"

namespace UI
{
	class MessageBox :
		public UIElement, public IDragable
	{
	private:
		std::map<std::string, sf::Text*> _texts; // texts
		std::map<std::string, UI::Button*> _buttons; // buttons
		UI::Button* _closeButton; // close button (separated from others)

		sf::RectangleShape* _header; // upper drag zone

		bool _isActive; // is box active

		void updateElementsPosition();

	public:
		MessageBox(GameDataRef data, std::string title, std::string info, sf::Vector2f position);
		~MessageBox();

		// Returns true if messagebox is active
		bool isActive() const;

		// Sets messagebox position
		void setPosition(sf::Vector2f position) override;

		// basic functions
		void update(float deltaTime) override;
		void render() const override;

	};
}
