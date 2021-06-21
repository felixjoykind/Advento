#pragma once

#include "engine/defenitions/UI_DEFENITIONS.h"
#include "engine/ui/IHoverable.h"
#include "engine/ui/IDragable.h"
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
		public UIElement, public IHoverable, public IDragable
	{
	private:
		// vector of elements scroller is connected to
		std::vector<T*>& _content;

		// input data
		bool _isActive; // is element active
		ScrollbarBounds _bounds; // scroller bounds
		int _mouseWheelDelta = 0; // mouse wheel delta (for wheel scrolling logic)

		// data for dragging element with mouse
		float _elementHeight; // height of type of element scrollbar is connected to

		// colors data
		sf::Color _idleColor;
		sf::Color _hoveredColor;
		sf::Color _pressedColor;

		// max visible elements
		unsigned int MAX_VISIBLE_ELEMENTS = 8; // 8 by default

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

		void updateContentPosition();

		// basic functions
		void handleInput(sf::Event ev);
		void update(float deltaTime) override;
		void render() const override;

	};

	template<class T>
	Scroller<T>::Scroller(GameDataRef data, sf::Vector2f size, sf::Vector2f pos, ScrollbarBounds bounds, std::vector<T*>& content)
		:UIElement(data, size, pos), IHoverable(this), _content(content), IDragable(this),
		_isActive(false), _bounds(bounds)
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
		this->setPosition({ this->getPosition().x, this->_bounds.min });

		// is active based on container input
		if (this->_content.size() > 0)
		{
			this->_elementHeight = this->_content[0]->getShape().getSize().y;
			this->MAX_VISIBLE_ELEMENTS = this->_data->winConfig.height / (unsigned int)this->_elementHeight;
			if (this->_content.size() > this->MAX_VISIBLE_ELEMENTS - 1)
			{
				this->_isActive = true;

				// set size based on container size
				this->_shape->setSize(
					{
						this->_shape->getSize().x,
						float(this->_elementHeight)
					}
				);
			}

			this->updateContentPosition();
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
	void Scroller<T>::updateContentPosition()
	{
		// claculate new position for content
		for (unsigned int i = 0; i < this->_content.size(); i++)
		{
			auto& elem = this->_content[i]; // current element

			// calculating element y poisition based on scrollbar y position
			elem->setPosition(
				{
					// 2210 - 1950
					elem->getPosition().x,
					float(i * (this->_elementHeight + 10.f) + WORLD_PLATES_OFFSET) -
					(this->getPosition().y - this->_bounds.min) * (this->_content.size() / this->MAX_VISIBLE_ELEMENTS + 1.f) + WORLD_PLATE_OFFSET_FROM_TOP
				}
			);
		}
	}

	template<class T>
	void Scroller<T>::handleInput(sf::Event ev)
	{
		if (!this->_isActive) return;

		// for mouse buttons events
		if (ev.type == sf::Event::MouseButtonPressed)
		{
			if (InputManager::isElementPressed(this, this->_data->window, sf::Mouse::Left))
			{
				this->setBackgroundColor(this->_pressedColor); // update background color
			}
		}
		else if (ev.type == sf::Event::MouseButtonReleased)
		{
			this->setBackgroundColor(this->_idleColor);
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

		IDragable::update(deltaTime, this->_data->window);
		if (IDragable::dragStarted())
		{
			this->_shape->move(
				0.f,
				IDragable::getOffset().y
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

		// if we are moving somewhere - update connected elements positions
		if (this->getPosition().y > this->_bounds.min &&
			this->getPosition().y + this->_shape->getSize().y < this->_bounds.max)
		{
			// claculate new position for content
			this->updateContentPosition();
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
