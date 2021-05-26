#include "GenerationState.h"

#include "GameState.h"

GenerationState::GenerationState(GameDataRef data)
	:_data(data), _seedTextBox(new UI::Textbox(this->_data, { 100.f, 100.f }, { 400.f, 60.f }))
{
}

GenerationState::~GenerationState()
{
}

void GenerationState::Init()
{
	// init textbox
}

void GenerationState::HandleInput()
{
	sf::Event ev;
	while (_data->window.pollEvent(ev))
	{
		// if closed
		if (ev.type == sf::Event::Closed)
			this->_data->window.close();

		// if key pressed
		if (ev.type == sf::Event::KeyPressed)
		{
			// exit
			if (ev.key.code == sf::Keyboard::Escape)
				this->_data->states.RemoveState(); // removing current state
		}

		// textbox input handleing
		this->_seedTextBox->handleInput(ev);
	}
}

void GenerationState::Update(float deltaTime)
{
	// update textbox
	this->_seedTextBox->update(deltaTime);
}

void GenerationState::Render() const
{
	this->_data->window.clear(sf::Color(115, 115, 115));

	// render textbox
	this->_seedTextBox->render();

	this->_data->window.display();
}
