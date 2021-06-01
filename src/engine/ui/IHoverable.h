#pragma once

#include "gamedata/InputManager.h"

namespace UI
{
	class IHoverable
	{
	private:
		UIElement* _element;

	protected:
		bool _isHovered = false;

	public:
		IHoverable(UIElement* element)
			:_element(element)
		{ }
		~IHoverable() { }

		// Returns true is element is hovered
		bool isHovered() const { return this->_isHovered; }

		void update(float deltaTime, const sf::Window& window)
		{
			if (InputManager::isElementHovered(this->_element, window))
			{
				this->_isHovered = true;
			}
			else
				this->_isHovered = false;
		}

	};
}
