#pragma once

#include "elements/UIElement.h"
#include "gamedata/InputManager.h"

#define DRAG_MOUSE_BUTTON sf::Mouse::Button::Left

namespace UI
{
	class IDragable
	{
	private:
		UIElement* _element;

		// drag data
		bool _gotPressed = false;
		bool _dragStarted = false;
		float _lastMouseX = 0.f;
		float _lastMouseY = 0.f;

		sf::Vector2f _offset; // element offset

	public:
		IDragable(UIElement* element)
			:_element(element)
		{ }
		~IDragable() { }

		// Returns true if drag started
		bool dragStarted() const { return this->_dragStarted; }

		// Returns drag offset
		sf::Vector2f getOffset() const { return this->_offset; }

		void update(float deltaTime, const sf::Window& window)
		{
			// if element got pressed
			if (InputManager::isElementPressed(this->_element, window, DRAG_MOUSE_BUTTON))
			{
				this->_gotPressed = true;
			}
			else if (!InputManager::isMouseButtonPressed(DRAG_MOUSE_BUTTON))
			{
				this->_gotPressed = false;
			}

			if (this->_gotPressed)
			{
				// mouse position
				auto mouse_pos = InputManager::getMousePosition(window);

				if (!this->_dragStarted)
				{
					// start drag
					this->_dragStarted = true;
				}
				else
				{
					// update element offset
					this->_offset = {
						float(mouse_pos.x) - this->_lastMouseX,
						float(mouse_pos.y) - this->_lastMouseY
					};
				}

				// update last mouse position
				this->_lastMouseX = float(mouse_pos.x);
				this->_lastMouseY = float(mouse_pos.y);
			}
			else
			{
				// stop drag
				this->_dragStarted = false;
			}
		}
	};
}