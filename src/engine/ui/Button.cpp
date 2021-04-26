#include "Button.h"

namespace UI
{
	Button::Button(
		GameDataRef data, sf::Vector2f pos, sf::Vector2f size,
		sf::Color back_idle, sf::Color back_hover, sf::Color back_click,
		sf::Color text_idle, sf::Color text_hover, sf::Color text_click,
		sf::Text content)
		:_data(data), _background(new sf::RectangleShape(size)), _content(new sf::Text(content)),
		idle_color(back_idle), hover_color(back_hover), click_color(back_click),
		text_idle(text_idle), text_hover(text_hover), text_click(text_click)
	{
		this->_state = ButtonState::BTN_IDLE;
		this->_background->setFillColor(back_idle);
		this->_content->setFillColor(text_idle);

		this->_background->setPosition(pos);
		this->_content->setPosition(
			sf::Vector2f(
				_background->getPosition().x + (_background->getGlobalBounds().width / 2.f) - _content->getGlobalBounds().width / 2.f,
				_background->getPosition().y + (_background->getGlobalBounds().height / 2.f) - _content->getGlobalBounds().height
			)
		);
	}

	Button::~Button()
	{
		delete this->_background;
		delete this->_content;
	}

	bool Button::isPressed(sf::Mouse::Button button) const
	{
		return this->_background->getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(_data->window)))
			&& sf::Mouse::isButtonPressed(button);
	}

	void Button::update(float deltaTime)
	{
		this->_state = ButtonState::BTN_IDLE;;

		// if button is hovered
		if (this->_background->getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(_data->window))))
		{
			this->_state = ButtonState::BTN_HOVER;
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				this->_state = ButtonState::BTN_CLICK;
			}
		}

		// setting color based on state
		switch (this->_state)
		{
		case ButtonState::BTN_IDLE:
			this->_background->setFillColor(this->idle_color);
			this->_content->setFillColor(this->text_idle);
			break;
		case ButtonState::BTN_HOVER:
			this->_background->setFillColor(this->hover_color);
			this->_content->setFillColor(this->text_hover);
			break;
		case ButtonState::BTN_CLICK:
			this->_background->setFillColor(this->click_color);
			this->_content->setFillColor(this->text_click);
			break;
		}
	}

	void Button::render() const
	{
		_data->window.draw(*this->_background);
		_data->window.draw(*this->_content);
	}

}
