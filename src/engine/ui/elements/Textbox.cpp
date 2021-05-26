#include "Textbox.h"

#include <SFML/Graphics.hpp>

constexpr int GUI_TEXT_BACKSPACE = 8; // backspace unicode

namespace UI
{
	Textbox::Textbox(GameDataRef data, sf::Vector2f pos, sf::Vector2f size)
		:UIElement(data, size, pos), ISelectable(this),
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

	void Textbox::setMaxCharacters(unsigned int maxCharacters)
	{
		this->_maxCharacters = maxCharacters;
	}

	void Textbox::handleInput(const sf::Event ev)
	{
		if (ev.type == sf::Event::TextEntered)
		{
			if (ISelectable::getSelectionType() == SelectionType::SELECTED)
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

				// update cursor position
				this->_cursor->setPosition(
					{
						this->_text->getPosition().x + this->_text->getGlobalBounds().width + 3.f,
						this->_text->getPosition().y
					}
				);
			}
		}
	}

	void Textbox::update(float deltaTime)
	{
		// update
		ISelectable::update(deltaTime, this->_data->window);

		// change textbox based on selection type
		switch (ISelectable::getSelectionType())
		{
		case SelectionType::NONE:
			this->_shape->setOutlineThickness(0.f);
			break;
		case SelectionType::HOVERED:
			this->_shape->setOutlineColor(sf::Color(190, 190, 190));
			this->_shape->setOutlineThickness(2.f);
			break;
		case SelectionType::SELECTED:
			this->_shape->setOutlineColor(sf::Color::White);
			this->_shape->setOutlineThickness(2.f);
			break;
		}
	}

	void Textbox::render() const
	{
		// render
		this->_data->window.draw(*this->_shape);
		this->_data->window.draw(*this->_text);

		// render cursor of selected
		if (ISelectable::getSelectionType() == SelectionType::SELECTED)
			this->_data->window.draw(*this->_cursor);
	}
}
