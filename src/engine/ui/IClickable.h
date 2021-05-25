#pragma once

#include "elements/UIElement.h"
#include "gamedata/InputManager.h"

namespace UI
{
	enum class ClickType { SINGLE = 0, DOUBLE, NONE };

	class IClickable
	{
	private:
		UIElement* _element;

	protected:
		float _keyTime;
		sf::Clock _keytimeClock;

	public:
		IClickable(UIElement* element, float keyTime)
			: _element(element), _keyTime(keyTime)
		{
			// starting _keyTimeClock
			this->_keytimeClock.restart();
		}
		~IClickable() { }

		ClickType GetClick(const sf::Window& window, sf::Mouse::Button button)
		{
			if (InputManager::isElementPressed(_element, window, button))
			{
				// if double click
				if (this->_keytimeClock.getElapsedTime().asSeconds() < this->_keyTime)
				{
					this->_keytimeClock.restart(); // restart clock
					return ClickType::DOUBLE;
				}
				else // single click
				{
					this->_keytimeClock.restart(); // restart clock
					return ClickType::SINGLE;
				}
			}
			return ClickType::NONE;
		}
	};
}

