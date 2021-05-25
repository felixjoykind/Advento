#include "Textbox.h"

namespace UI
{
	Textbox::Textbox(GameDataRef data, sf::Vector2f pos, sf::Vector2f size)
		:UIElement(data, size, pos), _text(new sf::Text("", _data->assets.GetFont("menu font")))
	{
		// text (content) position
		_text->setPosition(
			this->_shape->getPosition().x + 5.f,
			this->_shape->getPosition().y + 5.f
		);
	}

	Textbox::~Textbox()
	{
		delete this->_text;
	}

	void Textbox::handleInput()
	{
		sf::Event ev;
		while (this->_data->window.pollEvent(ev))
		{
			if (ev.type == sf::Event::KeyPressed)
			{
				if (ev.key.code >= sf::Keyboard::A && ev.key.code <= sf::Keyboard::Z)
				{
					this->_content += ev.key.code;
					this->_text->setString(this->_content);
				}
			}
		}
	}

	void Textbox::update(float deltaTime)
	{
		// update
	}

	void Textbox::render() const
	{
		// render
		this->_data->window.draw(*this->_shape);
		this->_data->window.draw(*this->_text);
	}
}
