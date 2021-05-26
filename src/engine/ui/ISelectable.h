#pragma once

#include "gamedata/InputManager.h"

namespace UI
{
	enum class SelectionType { NONE = 0, HOVERED, SELECTED };

	class ISelectable
	{
	private:
		UIElement* _element;

	protected:
		SelectionType _type = SelectionType::NONE;

	public:
		ISelectable(UIElement* element)
			:_element(element)
		{ }
		~ISelectable() { }

		// Returns current UI element state
		SelectionType getSelectionType() const
		{
			return this->_type;
		}

		void update(float deltaTime, const sf::Window& window)
		{
			// choosing state (hovered / selected)
			if (this->_type != SelectionType::SELECTED)
			{
				if (InputManager::isElementPressed(this->_element, window, sf::Mouse::Left))
				{
					this->_type = SelectionType::SELECTED;
				}
				else if (InputManager::isElementHovered(this->_element, window))
				{
					this->_type = SelectionType::HOVERED;
				}
				else { this->_type = SelectionType::NONE; }
			}
			else if (InputManager::isMouseButtonPressed(sf::Mouse::Left)
				&& !InputManager::isElementHovered(this->_element, window))
				this->_type = SelectionType::NONE;
		}

	};
}