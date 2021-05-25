#pragma once

#include "gamedata/InputManager.h"

namespace UI
{
	class ISelectable
	{
	private:
		UIElement* _element;

	protected:
		bool _selected = false;

	public:
		ISelectable(UIElement* element)
			:_element(element)
		{ }
		~ISelectable() { }

		// getters
		bool isSelected() const { return this->_selected; }

		void update(float deltaTime, const sf::Window& window)
		{
			// choosing state (hovered / selected)
			if (!_selected)
			{
				if (InputManager::isElementPressed(this->_element, window, sf::Mouse::Left))
				{
					this->_selected = true;
					this->_element->_shape->setOutlineColor(sf::Color::White);
					this->_element->_shape->setOutlineThickness(2.f);
				}
				else if (InputManager::isElementHovered(this->_element, window))
				{
					this->_element->_shape->setOutlineColor(sf::Color(190, 190, 190));
					this->_element->_shape->setOutlineThickness(2.f);
				}
				else
				{
					this->_element->_shape->setOutlineThickness(0.f);
				}
			}
			else if (InputManager::isMouseButtonPressed(sf::Mouse::Left)
				&& !InputManager::isElementHovered(this->_element, window))
				this->_selected = false;
		}

	};
}