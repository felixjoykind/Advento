#include "Button.h"

namespace UI
{
	Button::Button(
		GameDataRef data, sf::Vector2f pos, sf::Vector2f size,
		sf::Color back_idle, sf::Color back_hover, sf::Color back_click,
		sf::Color text_idle, sf::Color text_hover, sf::Color text_click,
		sf::Text content)
		:UIElement(data, size, pos), IHoverable(this), IClickable(this, 0.2f), _content(new sf::Text(content)),
		idle_color(back_idle), hover_color(back_hover), click_color(back_click),
		text_idle(text_idle), text_hover(text_hover), text_click(text_click)
	{
		this->_shape->setFillColor(back_idle);
		this->_content->setFillColor(text_idle);

		this->_shape->setPosition(pos);
		this->_content->setPosition(
			sf::Vector2f(
				_shape->getPosition().x + (_shape->getGlobalBounds().width / 2.f) - _content->getGlobalBounds().width / 2.f,
				_shape->getPosition().y + (_shape->getGlobalBounds().height / 2.f) - _content->getGlobalBounds().height
			)
		);
	}

	Button::~Button()
	{
		delete this->_content;
	}

	bool Button::isPressed(sf::Mouse::Button button)
	{
		return IClickable::GetClick(this->_data->window, button) == ClickType::SINGLE;
	}

	void Button::update(float deltaTime)
	{
		IHoverable::update(deltaTime, this->_data->window);

		// if button is hovered
		if (IHoverable::_isHovered)
		{
			this->_shape->setFillColor(this->hover_color);
			this->_content->setFillColor(this->text_hover);

			// if also pressed
			if (InputManager::isMouseButtonPressed(sf::Mouse::Button::Left))
			{
				this->_shape->setFillColor(this->click_color);
				this->_content->setFillColor(this->text_click);
			}
		}
		else
		{
			this->_shape->setFillColor(this->idle_color);
			this->_content->setFillColor(this->text_idle);
		}
	}

	void Button::render() const
	{
		_data->window.draw(*this->_shape);
		_data->window.draw(*this->_content);
	}

}
