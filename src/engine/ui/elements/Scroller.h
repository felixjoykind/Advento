#pragma once

#include "engine/defenitions/UI_DEFENITIONS.h"
#include "engine/ui/IHoverable.h"
#include "engine/LOG.h"
#include "engine/Math.h"

#define SCROLL_SPEED 1500

namespace UI
{
	// Represents bounds of scrollbar element
	struct ScrollbarBounds
	{
		float min, max;
	};

	template <class T> // type of content (UIElements only)
	class Scroller :
		public UIElement, public IHoverable
	{
	private:
		// vector of elements scroller is connected to
		std::vector<T*>& _content;

		// data
		bool _gotPressed;
		bool _isActive;
		ScrollbarBounds _bounds;
		int _mouseWheelDelta = 0; // mouse wheel delta (for wheel scrolling logic)

		// colors data
		sf::Color _idleColor;
		sf::Color _hoveredColor;
		sf::Color _pressedColor;

		// max visible elements
		unsigned int MAX_VISIBLE_ELEMENTS = 8;

	public:
		Scroller(GameDataRef data, sf::Vector2f size, sf::Vector2f pos, ScrollbarBounds bounds, std::vector<T*>& content);
		~Scroller();

		void init();

		// Sets idle color
		void setIdleColor(const sf::Color color);

		// Sets hovered color
		void setHoveredColor(const sf::Color color);

		// Sets pressed color
		void setPressedColor(const sf::Color color);

		// basic functions
		void handleInput(sf::Event ev);
		void update(float deltaTime) override;
		void render() const override;

	};

	template<class T>
	Scroller<T>::Scroller(GameDataRef data, sf::Vector2f size, sf::Vector2f pos, ScrollbarBounds bounds, std::vector<T*>& content)
		:UIElement(data, size, pos), IHoverable(this), _content(content),
		_gotPressed(false), _isActive(false), _bounds(bounds)
	{
		static_assert(std::is_base_of<UIElement, T>::value,
			"Scroller can only be connected to UIElement or derived from it class (Scroller.h, in contructor)");

		// settings colors by default
		this->_idleColor = sf::Color(34, 29, 28, 200);
		this->_hoveredColor = sf::Color(34, 29, 28, 220);
		this->_pressedColor = sf::Color(34, 29, 28, 255);
	}

	template<class T>
	Scroller<T>::~Scroller()
	{
	}

	template<class T>
	inline void Scroller<T>::init()
	{
		// set active based on container input
		if (this->_content.size() > 0)
		{
			auto element_height = this->_content[0]->getShape().getSize().y;
			this->MAX_VISIBLE_ELEMENTS = this->_data->winConfig.height / (unsigned int)element_height;
			if (this->_content.size() > this->MAX_VISIBLE_ELEMENTS - 1)
			{
				this->_isActive = true;

				// set size based on container size
				this->_shape->setSize(
					{
						this->_shape->getSize().x,
						float(element_height)
					}
				);
			}
		}
		else
			this->_isActive = false;
	}

	template<class T>
	void Scroller<T>::setIdleColor(const sf::Color color)
	{
		this->_idleColor = color;
	}

	template<class T>
	void Scroller<T>::setHoveredColor(const sf::Color color)
	{
		this->_hoveredColor = color;
	}

	template<class T>
	void Scroller<T>::setPressedColor(const sf::Color color)
	{
		this->_pressedColor = color;
	}

	template<class T>
	inline void Scroller<T>::handleInput(sf::Event ev)
	{
		if (!this->_isActive) return;

		// for mouse buttons events
		if (ev.type == sf::Event::MouseButtonPressed)
		{
			if (InputManager::isElementPressed(this, this->_data->window, sf::Mouse::Left))
			{
				this->setBackgroundColor(this->_pressedColor); // update background color
				this->_gotPressed = true;
			}
		}
		else if (ev.type == sf::Event::MouseButtonReleased)
		{
			this->setBackgroundColor(this->_idleColor);
			this->_gotPressed = false;
		}

		// for mouse wheel events
		if (ev.type == sf::Event::MouseWheelMoved)
		{
			// update wheel delta
			this->_mouseWheelDelta = ev.mouseWheel.delta;
		}
	}

	template<class T>
	void Scroller<T>::update(float deltaTime)
	{
		if (!this->_isActive) return;

		// hovering logic
		IHoverable::update(deltaTime, this->_data->window);
		if (IHoverable::_isHovered) // if hovered
		{
			this->setBackgroundColor(this->_hoveredColor);
		}
		else
		{
			// default/idle color
			this->setBackgroundColor(this->_idleColor);
		}

		if (this->_gotPressed) // if got pressed
		{
			this->setBackgroundColor(this->_pressedColor); // update background color

			// move scroller with mouse
			auto mouse_pos = sf::Mouse::getPosition(this->_data->window);
			this->_shape->setPosition(
				this->_shape->getPosition().x,
				float(mouse_pos.y) - this->_shape->getSize().y / 2.f
			);
		}

		// update scrollbar position with wheel input
		this->setPosition(
			{
				this->_shape->getPosition().x,
				this->_shape->getPosition().y - float(this->_mouseWheelDelta * SCROLL_SPEED) * deltaTime
			}
		);
		// if wheel was moved
		if (this->_mouseWheelDelta != 0)
		{
			// stop it
			this->_mouseWheelDelta = 0;
		}

		// if we are moving somewhere - update elements positions
		if (this->getPosition().y > this->_bounds.min &&
			this->getPosition().y + this->_shape->getSize().y < this->_bounds.max)
		{
			// claculate new position for content
			for (unsigned int i = 0; i < this->_content.size(); i++)
			{
				auto& elem = this->_content[i]; // current element

				// calculating element poisition based on scrollbar position
				elem->setPosition(
					{
						elem->getPosition().x,
						float(i * 110.f + WORLD_PLATES_OFFSET) -
						(this->getPosition().y - this->_bounds.min) * (this->_content.size() / this->MAX_VISIBLE_ELEMENTS)
					}
				);
			}
		}

		// clamp scrollbar position with bounds
		this->_shape->setPosition(
			{
				_shape->getPosition().x,
				Math::clamp<float>(_bounds.min, _bounds.max - _shape->getSize().y, _shape->getPosition().y)
			}
		);
	}

	template<class T>
	void Scroller<T>::render() const
	{
		if (!this->_isActive) return;

		// base UIElement class render function call
		UIElement::render();
	}
}
