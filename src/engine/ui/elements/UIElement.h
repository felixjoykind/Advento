#pragma once

#include "Game.h"

namespace UI
{
	// Base class for all ui elements
	class UIElement
	{
	protected:
		sf::RectangleShape* _shape;
		GameDataRef _data;

	public:
		UIElement(GameDataRef data, sf::Vector2f size, sf::Vector2f position) 
			: _data(data), _shape(new sf::RectangleShape(size))
		{
			_shape->setPosition(position);
		}
		~UIElement()
		{
			// delete shape
			delete this->_shape;
		}

		// getters
		const sf::RectangleShape& getShape() const
		{
			return *this->_shape;
		}

		// setters
		void setBackgroundColor(const sf::Color& color)
		{
			this->_shape->setFillColor(color);
		}

		// basic functions
		virtual void update(float deltaTime) { }
		virtual void render() const { this->_data->window.draw(*this->_shape); };

		friend class IClickable;
		friend class ISelectable;
		friend class IHoverable;

	};
}
