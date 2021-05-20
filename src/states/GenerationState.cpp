#include "GenerationState.h"

#include "GameState.h"

GenerationState::GenerationState(GameDataRef data)
	:_data(data)
{
}

GenerationState::~GenerationState()
{
}

void GenerationState::Init()
{
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
			if (ev.key.code == sf::Keyboard::Escape)
				this->_data->states.RemoveState(); // removing current state

			//	DEBUG CODE REMOVE LATER!!!
			/*if (ev.key.code == sf::Keyboard::Space)
				this->_data->states.AddState(StateRef(new GameState(this->_data)), true);*/
		}
	}
}

void GenerationState::Update(float deltaTime)
{
	// TODO: delete all states when adding game state

	// update
}

void GenerationState::Render() const
{
	this->_data->window.clear();

	this->_data->window.display();
}
