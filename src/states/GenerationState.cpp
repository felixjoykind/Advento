#include "GenerationState.h"

#include <filesystem>
#include <random>
#include <windows.h>

#include "engine/defenitions/BASIC_WORLD_SETTINGS.h"
#include "engine/defenitions/PATH_DEFENITIONS.h"
#include "engine/defenitions/COLORS.h"

#include "engine/tiles/TileMap.h"

#include "engine/SpriteManipulator.h"

namespace fs = std::filesystem;

GenerationState::GenerationState(GameDataRef data)
	:_data(data), _background(new sf::Sprite(this->_data->assets.GetTexture("generation menu background")))
{
}

GenerationState::~GenerationState()
{
}

void GenerationState::Init()
{
	// stretching background image to window size
	SpriteManipulator::Stretch(this->_background,
		{ float(this->_data->winConfig.width), float(this->_data->winConfig.height) });

	// init textboxes
	this->_textboxes["WORLD_NAME"] = new UI::Textbox(this->_data, 
		{
			float(this->_data->winConfig.width / 2) - 200.f,
			100.f 
		},
		{ 400.f, 60.f }
	);

	// choosing new world name based on existing worlds
	std::string world_name = "New World";
	unsigned int i = 0;
	while (fs::exists((WORLDS_DIR + std::string("\\") + world_name)))
	{
		++i;
		world_name = "New World " + std::to_string(i);
	}

	this->_textboxes["WORLD_NAME"]->setString(world_name);

	this->_textboxes["WORLD_SEED"] = new UI::Textbox(this->_data,
		{
			float(this->_data->winConfig.width / 2) - 200.f,
			210.f
		},
		{ 400.f, 60.f }
	);

	// init titles
	this->_titles["WORLD_NAME"] = new sf::Text("New world name", this->_data->assets.GetFont("menu title font"));
	this->_titles["WORLD_NAME"]->setPosition(
		{
			float(this->_data->winConfig.width / 2) - 200.f,
			60.f
		}
	);

	this->_titles["WORLD_SEED"] = new sf::Text("Seed", this->_data->assets.GetFont("menu title font"));
	this->_titles["WORLD_SEED"]->setPosition(
		{
			float(this->_data->winConfig.width / 2) - 200.f,
			170.f
		}
	);

	// init buttons
	this->_buttons["BACK"] = new UI::Button(this->_data, { 60.f, 60.f }, { 100.f, 50.f }, 
		BLANK_COLOR, BLANK_COLOR, BLANK_COLOR,
		sf::Color(192, 192, 192), sf::Color::White, sf::Color::White,
		sf::Text("Back", this->_data->assets.GetFont("menu font"), 40)
	);

	this->_buttons["CREATE"] = new UI::Button(this->_data, 
		{
			float(this->_data->winConfig.width / 2) - 80.f,
			float(this->_data->winConfig.height / 2) - 40.f
		},
		{ 160.f, 60.f },
		BLANK_COLOR, BLANK_COLOR, BLANK_COLOR,
		sf::Color(192, 192, 192), sf::Color::White, sf::Color::White,
		sf::Text("Create", this->_data->assets.GetFont("menu font"), 50)
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
			// if exit
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
	if (this->_buttons.at("BACK")->isPressed(sf::Mouse::Left))
	{
		this->_data->states.RemoveState(); // removing current state
	}

	if (this->_buttons.at("CREATE")->isPressed(sf::Mouse::Left))
	{
		// check if world name contains forbidden characters
		std::string world_name = this->_textboxes.at("WORLD_NAME")->getString();
		if (world_name.find('/') != std::string::npos ||
			world_name.find('\\') != std::string::npos || 
			world_name.find('|') != std::string::npos || 
			world_name.find('"') != std::string::npos || 
			world_name.find('<') != std::string::npos || 
			world_name.find('>') != std::string::npos ||
			world_name.find(':') != std::string::npos || 
			world_name.find('?') != std::string::npos ||
			world_name.find('*') != std::string::npos)
		{
			// tell user that world name is forbidden
			MessageBoxA(
				NULL, 
				"World name can't contain characters like: / \\ | ? * \" < > :", "Forbidden world name", 
				MB_OK | MB_ICONEXCLAMATION
			);
			return;
		}
		else if (world_name.length() == 0)
		{
			MessageBoxA(
				NULL,
				"World name cannot be empty", "Forbidden world name",
				MB_OK | MB_ICONEXCLAMATION
			);
			return;
		}

		// check if world with same name already exists and edit world name based on the results
		unsigned int i = 0;
		while (fs::exists((WORLDS_DIR + std::string("\\") + world_name)))
		{
			++i;
			world_name = this->_textboxes.at("WORLD_NAME")->getString() + std::to_string(i);
		}

		// generate random seed if empty
		this->_textboxes.at("WORLD_SEED")->setString(Engine::MapGenerator::getRandomSeed(16));

		Engine::TileMap map(this->_data, BASIC_WORLD_SIZE_X, BASIC_WORLD_SIZE_Y, nullptr); // new map

		// generating world with basic world generation settings and custom name, seed
		map.generate(
			{
				world_name,
				BASIC_WORLD_SIZE_X, BASIC_WORLD_SIZE_Y, 
				0.4f, 4, 4, 5
			}
		);
		// save tilemap into world folder
		map.save_to(
			{
				world_name,
				WORLDS_DIR + std::string("\\") + world_name
			}
		);

		// go back to worlds list
		this->_data->states.RemoveState(); // remove current state
	}

	// update textbox
	for (auto& [name, textbox] : this->_textboxes)
		textbox->update(deltaTime);

	// update buttons
	for (auto& [name, button] : this->_buttons)
		button->update(deltaTime);
}

void GenerationState::Render() const
{
	this->_data->window.clear(sf::Color(115, 115, 115));

	// render background
	this->_data->window.draw(*this->_background);

	// render textbox
	for (const auto& [name, textbox] : this->_textboxes)
		textbox->render();

	// render titles
	for (const auto& [name, title] : this->_titles)
		this->_data->window.draw(*title);

	// render buttons
	for (const auto& [name, button] : this->_buttons)
		button->render();

	this->_data->window.display();
}
