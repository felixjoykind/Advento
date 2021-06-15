#include "MessageBox.h"
#include "engine/LOG.h"

#define MIN_WINDOW_WIDTH 200.f
#define MIN_WINDOW_HEIGHT 200.f

#define DRAGZONE_HEIGHT 30.f

#define CLOSE_BUTTON_SIZE 30.f
#define BUTTON_WIDTH 80.f
#define BUTTON_HEIGHT 40.f

namespace UI
{
	void MessageBox::updateElementsPosition()
	{
		auto position = this->getPosition();

		this->_header->setPosition(position);

		this->_texts.at("TITLE")->setPosition(
			position.x + 5.f,
			position.y
		);

		this->_texts.at("INFO")->setPosition(
			position.x + 5.f,
			position.y + _header->getSize().y + 5.f
		);

		// move buttons
		this->_closeButton->setPosition(
			{
				_shape->getPosition().x + _shape->getSize().x - CLOSE_BUTTON_SIZE,
				_shape->getPosition().y
			}
		);
		for (auto& [name, button] : this->_buttons)
		{
			button->setPosition(
				{
					_shape->getPosition().x + 10.f + (_buttons.size() - 1) * BUTTON_WIDTH,	// x
					_shape->getPosition().y + _shape->getSize().y - 50.f				// y
				}
			);
		}
	}

	MessageBox::MessageBox(GameDataRef data, std::string title, std::string info, sf::Vector2f position)
		:UIElement(data, { MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT }, position), IDragable(this),
		_header(new sf::RectangleShape({ MIN_WINDOW_WIDTH, DRAGZONE_HEIGHT })),
		_isActive(true)
	{
		// init main box
		UIElement::setBackgroundColor(sf::Color(0, 0, 0, 200));

		// init drag zone
		this->_header->setPosition(position);
		this->_header->setFillColor(sf::Color::Black);

		// init texts
		this->_texts["TITLE"] = new sf::Text(
			title, this->_data->assets.GetFont("SegoeUI Semibold"), 20U
		);
		this->_texts.at("TITLE")->setPosition(position.x + 5.f, position.y);

		this->_texts["INFO"] = new sf::Text(
			info, this->_data->assets.GetFont("SegoeUI Regular"), 18U
		);
		this->_texts.at("INFO")->setPosition(
			position.x + 5.f,
			position.y + _header->getSize().y + 5.f
		);
		
		// init buttons
		this->_closeButton = new UI::Button(
			this->_data,
			{ _shape->getPosition().x + _shape->getSize().x - CLOSE_BUTTON_SIZE, _shape->getPosition().y }, 
			{ CLOSE_BUTTON_SIZE, CLOSE_BUTTON_SIZE },
			sf::Color(0, 0, 0, 180), sf::Color::Black, sf::Color::Black,
			sf::Color(255, 255, 255, 200), sf::Color::White, sf::Color::White,
			sf::Text("X", _data->assets.GetFont("SegoeUI Regular"), 18U)
		);
		
		this->_buttons["OK"] = new UI::Button(
			this->_data,
			{ _shape->getPosition().x + 10.f + _buttons.size() * BUTTON_WIDTH, _shape->getPosition().y + _shape->getSize().y - 50.f },
			{ BUTTON_WIDTH, BUTTON_HEIGHT },
			sf::Color(0, 0, 0, 180), sf::Color::Black, sf::Color::Black,
			sf::Color(255, 255, 255, 200), sf::Color::White, sf::Color::White,
			sf::Text("OK", _data->assets.GetFont("SegoeUI Regular"), 18U)
		);

		// if title doesn't fit
		if (this->_texts.at("TITLE")->getGlobalBounds().width > MIN_WINDOW_WIDTH)
		{
			// resize
			auto new_width = this->_texts.at("TITLE")->getGlobalBounds().width + CLOSE_BUTTON_SIZE + 10.f;

			// update content size and position with new width
			this->_shape->setSize({ new_width, _shape->getSize().y });
			this->_header->setSize({ new_width, DRAGZONE_HEIGHT });
			this->_closeButton->setPosition(
				{
					_shape->getPosition().x + _shape->getSize().x - CLOSE_BUTTON_SIZE,
					_shape->getPosition().y
				}
			);
		}
		// if content doesn't fit
		if (this->_texts.at("INFO")->getGlobalBounds().height > 
			MIN_WINDOW_HEIGHT - DRAGZONE_HEIGHT - 50.f) // DOESNT WORK!!!
		{
			this->_shape->setSize(
				{
					this->_shape->getSize().x,
					this->_texts.at("INFO")->getGlobalBounds().height + DRAGZONE_HEIGHT + 60.f
				}
			);

			// update buttons positions
			// this->setPosition(this->getPosition()); // shitty code time
			this->updateElementsPosition();
		}
	}

	MessageBox::~MessageBox()
	{
		// delete text
		for (auto& [name, text] : this->_texts)
			delete text;
		this->_texts.clear();

		// delete buttons
		delete this->_closeButton; // close button
		for (auto& [name, button] : this->_buttons) // other buttons
			delete button;
		this->_buttons.clear();
	}

	bool MessageBox::isActive() const { return this->_isActive; }

	void MessageBox::setPosition(sf::Vector2f position)
	{
		// move all content
		UIElement::setPosition(position);

		this->updateElementsPosition();
	}

	void MessageBox::update(float deltaTime)
	{
		// drag logic
		IDragable::update(deltaTime, this->_data->window);

		if (IDragable::dragStarted())
		{
			this->_shape->move(IDragable::getOffset()); // move shape

			// also move all elements
			//this->setPosition(this->getPosition());
			this->updateElementsPosition();
		}

		// if OK button is pressed
		if (this->_buttons.at("OK")->isPressed(sf::Mouse::Button::Left)
			|| this->_closeButton->isPressed(sf::Mouse::Button::Left))
		{
			// box is not active anymore
			this->_isActive = false;
		}

		// update buttons
		this->_closeButton->update(deltaTime); // close button
		for (auto& [name, button] : this->_buttons) // other
			button->update(deltaTime);
	}

	void MessageBox::render() const
	{
		// render all content
		UIElement::render(); // basic render function call
		this->_data->window.draw(*this->_header); // render frag zone

		// render texts
		for (const auto& [name, text] : this->_texts)
			this->_data->window.draw(*text);

		// render buttons
		this->_closeButton->render(); // close button
		for (const auto& [name, button] : this->_buttons) // other
			button->render();

	}
}
