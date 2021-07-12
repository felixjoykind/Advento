#pragma once

#include "Game.h"

namespace UI
{
	// Base class for all ui elements
	class UIElement
	{
	protected:
		bool _isActive;
		sf::RectangleShape* _shape;
		GameDataRef _data;

	public:
		UIElement(GameDataRef data, sf::Vector2f size, sf::Vector2f position) 
			: _data(data), _isActive(true), _shape(new sf::RectangleShape(size))
		{
			_shape->setPosition(position);
		}
		~UIElement()
		{
			// delete shape
			delete this->_shape;
		}

		// Sets active
		void setActive(bool value)
		{
			this->_isActive = value;
		}

		// Sets element position
		// Virtual because element may contain other elements like Text 
		// or something else, that should also be moved
		virtual void setPosition(sf::Vector2f position)
		{
			this->_shape->setPosition(position);
		}

		// Returns true is element is active
		bool isActive() const { return this->_isActive; }

		// Returns true if element is in bounds
		bool isInBounds_Y(float min, float max)
		{
			if (this->getPosition().y > min &&
				this->getPosition().y < max)
			{
				return true;
			}
			return false;
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
		virtual void update(float deltaTime) { if (!this->_isActive) return; }
		virtual void render() const { this->_data->window.draw(*this->_shape); };

	};
}
