#pragma once

#include "gamedata/InputManager.h"

namespace UI
{
	// Provides selection logic
	class ISelectable
	{
	private:
		UIElement* _element;

	protected:
		bool _isSelected;

	public:
		ISelectable(UIElement* element)
			:_element(element), _isSelected(false)
		{ }
		~ISelectable() { }

		// Returns true if UIElement is selected
		bool isSelected() const { return this->_isSelected; }

		void update(float deltaTime, const sf::Window& window)
		{
			// choosing state (hovered / selected)
			if (InputManager::isElementPressed(this->_element, window, sf::Mouse::Left))
			{
				this->_isSelected = true;
			}
			else if (InputManager::isMouseButtonPressed(sf::Mouse::Left)
				&& !InputManager::isElementHovered(this->_element, window))
			{
				this->_isSelected = false;
			}
		}

	};
}