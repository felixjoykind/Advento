#pragma once

#include "engine/defenitions/UI_DEFENITIONS.h"
#include "engine/ui/IHoverable.h"
#include "engine/LOG.h"
#include "engine/Math.h"

namespace UI
{
	struct ScrollbarBounds
	{
		float min, max;
	};

	template <class T>
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

		// colors data
		sf::Color _idleColor;
		sf::Color _hoveredColor;
		sf::Color _pressedColor;

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
		:UIElement(data, size, pos), IHoverable(this), _content(content), _gotPressed(false), _isActive(false), _bounds(bounds)
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
			if (this->_content.size() * WORLD_PLATES_OFFSET + this->_content.size() * element_height > this->_bounds.max)
			{
				LOG(this->_content.size() * WORLD_PLATES_OFFSET + this->_content.size() * element_height);

				this->_isActive = true;

				// set size based on container size
				this->_shape->setSize(
					{
						this->_shape->getSize().x,
						float(this->_data->winConfig.height - this->_content.size() * 10)
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
	}

	template<class T>
	void Scroller<T>::update(float deltaTime)
	{
		if (!this->_isActive) return;

		// TODO: move scrollbar with mouse smoothly and corectly
		// TODO: implement mouse wheel logic

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

		// clamp scrollbar with it bounds
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
