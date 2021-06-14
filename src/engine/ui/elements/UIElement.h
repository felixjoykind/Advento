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

		// Sets element position
		// Virtual because element may contain other elements like Text 
		// or something else, that should also be moved
		virtual void setPosition(sf::Vector2f position)
		{
			this->_shape->setPosition(position);
		}

		// Returns element position
		sf::Vector2f getPosition() const { return this->_shape->getPosition(); }

		// Returns elment shape
		const sf::RectangleShape& getShape() const { return *this->_shape; }

		// Sets background color
		void setBackgroundColor(const sf::Color& color)
		{
			this->_shape->setFillColor(color);
		}

		// basic functions
		virtual void update(float deltaTime) { }
		virtual void render() const { this->_data->window.draw(*this->_shape); };

	};
}
