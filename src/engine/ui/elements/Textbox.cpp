#include "Textbox.h"

#include <SFML/Graphics.hpp>
#include <cassert>

constexpr int GUI_TEXT_BACKSPACE = 8; // backspace unicode

namespace UI
{
	Textbox::Textbox(GameDataRef data, sf::Vector2f pos, sf::Vector2f size)
		:UIElement(data, size, pos), IHoverable(this),  ISelectable(this),
		_text(new sf::Text("", _data->assets.GetFont("menu font"), 30U)),
		_cursor(new sf::RectangleShape({ 1.f, float(this->_text->getCharacterSize()) }))
	{
		// init background
		this->setBackgroundColor(sf::Color::Black);

		// text (content) position
		_text->setPosition(
			this->_shape->getPosition().x + 5.f,
			this->_shape->getPosition().y + this->_shape->getGlobalBounds().height / 2.f - float(this->_text->getCharacterSize() / 2)
		);

		this->_cursor->setPosition(
			{
				this->_text->getPosition().x + this->_text->getGlobalBounds().width,
				this->_text->getPosition().y - this->_text->getGlobalBounds().height
			}
		);
	}

	Textbox::~Textbox()
	{
		delete this->_text;
		delete this->_cursor;
	}

	std::string Textbox::getString() const
	{
		return this->_content;
	}

	bool Textbox::isEmpty() const
	{
		return this->_content.length() == 0;
	}

	void Textbox::setMaxCharacters(unsigned int maxCharacters)
	{
		this->_maxCharacters = maxCharacters;
	}

	void Textbox::setString(const std::string content)
	{
		assert(content.length() <= this->_maxCharacters && 
			"Too big string (Textbox.cpp, setString(std::string))");

		this->_content = content;
		this->_text->setString(content);
	}

	void Textbox::handleInput(const sf::Event ev)
	{
		if (ev.type == sf::Event::TextEntered)
		{
			if (ISelectable::_isSelected)
			{
				if (ev.text.unicode == GUI_TEXT_BACKSPACE) // if backspace
				{
					if (this->_content.length() > 0)
						this->_content.pop_back();
				}
				else // if just entering text
				{
					if (this->_content.length() < this->_maxCharacters) // if not already full
						this->_content += ev.text.unicode;
				}
				this->_text->setString(this->_content);
			}
		}
	}

	void Textbox::update(float deltaTime)
	{
		// update selection and hovering
		IHoverable::update(deltaTime, this->_data->window);
		ISelectable::update(deltaTime, this->_data->window);

		// change plate based on plate state
		this->_shape->setOutlineThickness(0.f); // idle
		if (IHoverable::_isHovered)
		{
			this->_shape->setOutlineColor(sf::Color(190, 190, 190));
			this->_shape->setOutlineThickness(2.f);
		}
		if (ISelectable::_isSelected)
		{
			this->_shape->setOutlineColor(sf::Color::White);
			this->_shape->setOutlineThickness(2.f);
		}

		// update cursor position
		this->_cursor->setPosition(
			{
				this->_text->getPosition().x + this->_text->getGlobalBounds().width + 3.f,
				this->_text->getPosition().y
			}
		);
	}

	void Textbox::render() const
	{
		// render
		this->_data->window.draw(*this->_shape);
		this->_data->window.draw(*this->_text);

		// render cursor of selected
		if (ISelectable::_isSelected)
			this->_data->window.draw(*this->_cursor);
	}
}
