#include "GenerationState.h"

#include <filesystem>
#include <iostream>
#include "engine/defenitions/PATH_DEFENTITIONS.h"

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
	// init textbox
	this->_textboxes["WORLD_NAME"] = new UI::Textbox(this->_data, 
		{
			float(this->_data->winConfig.width / 2) - 200.f,
			100.f 
		},
		{ 400.f, 60.f }
	);
	
	namespace fs = std::filesystem;

	std::string world_name = "New World";
	unsigned int i = 1;
	if (fs::exists(WORLDS_DIR + std::string("\\") + world_name))
	{
		world_name = "New World " + std::to_string(i);
		while (fs::exists((WORLDS_DIR + std::string("\\") + world_name)))
			++i;
	}

	this->_textboxes["WORLD_NAME"]->setString(world_name);

	this->_textboxes["WORLD_SEED"] = new UI::Textbox(this->_data,
		{
			float(this->_data->winConfig.width / 2) - 200.f,
			200.f
		},
		{ 400.f, 60.f }
	);
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
		for (auto& [name, textbox] : this->_textboxes)
			textbox->handleInput(ev);
	}
}

void GenerationState::Update(float deltaTime)
{
	// update textbox
	for (auto& [name, textbox] : this->_textboxes)
		textbox->update(deltaTime);
}

void GenerationState::Render() const
{
	this->_data->window.clear(sf::Color(115, 115, 115));

	// render textbox
	for (auto& [name, textbox] : this->_textboxes)
		textbox->render();

	this->_data->window.display();
}
