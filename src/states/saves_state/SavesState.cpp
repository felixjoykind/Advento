#include "SavesState.h"

#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "engine/defenitions/PATH_DEFENITIONS.h"
#include "engine/defenitions/COLORS.h"

#include "gamedata/InputManager.h"
#include "engine/SpriteManipulator.h"

#include "engine/tiles/TileMap.h"

#include "states/MainMenuState.h"
#include "states/game_state/GameState.h"

SavesState::SavesState(GameDataRef data)
	:_data(data), _background(new sf::Sprite(this->_data->assets.GetTexture("saves menu background"))),
	_downPanel(new sf::RectangleShape({ float(_data->winConfig.width), 95.f })),
	_scroller(new UI::Scroller<UI::WorldPlate>(
		_data, { 15.f, 400.f },
		{ float(_data->winConfig.width / 2 + _data->winConfig.width / 4) + 10.f, 20.f },
		{ WORLD_PLATE_OFFSET_FROM_TOP + WORLD_PLATES_OFFSET, float(_data->winConfig.height - 100) },
		_worlds)
	)
{
	// init down panel
	this->_downPanel->setPosition(
		{
			0.f,
			float(_data->winConfig.height - 95)
		}
	);
	this->_downPanel->setFillColor(sf::Color(240, 80, 64));
}

SavesState::~SavesState()
{
	// delete world plates
	for (size_t i = 0; i < this->_worlds.size(); i++)
		delete this->_worlds[i];
	this->_worlds.clear();

	// delete all buttons
	for (auto& [name, button] : this->_buttons)
		delete button;
	this->_buttons.clear();

	// delete background
	delete this->_background;

	// delete scrollbar
	delete this->_scroller;
}

void SavesState::Init()
{
	// if worlds dir doesn't exist
	namespace fs = std::filesystem;
	if (!std::filesystem::exists(std::filesystem::path(WORLDS_DIR)))
	{
		std::filesystem::create_directories(WORLDS_DIR); // creating directories
	}
	else
	{
		// load worlds
		this->RefreshWorldsList();
	}

	// stretching background image to window size
	SpriteManipulator::Stretch(this->_background,
		{ float(this->_data->winConfig.width), float(this->_data->winConfig.height) });

	// init titles
	this->_titles["WORLDS_TITLE"] = new sf::Text(
		"Worlds list", this->_data->assets.GetFont("menu title font"), 30U
	);
	this->_titles.at("WORLDS_TITLE")->setPosition(
		{ 
			float(_data->winConfig.width / 2) - _titles.at("WORLDS_TITLE")->getGlobalBounds().width / 2.f,
			20.f
		}
	); // pisition

	// if there are no worlds
	if (this->_worlds.size() == 0)
	{
		this->_titles["NO_WORLDS_TITLE"] = new sf::Text(
			"No worlds yet :(", this->_data->assets.GetFont("menu title font"), 60U
		);
		this->_titles.at("NO_WORLDS_TITLE")->setPosition(
			{
				float(_data->winConfig.width / 2) - _titles.at("NO_WORLDS_TITLE")->getGlobalBounds().width / 2.f,
				float(_data->winConfig.height / 2) - _titles.at("NO_WORLDS_TITLE")->getGlobalBounds().height / 2.f
			}
		); // pisition
	}

	// init buttons
	this->_buttons["NEW_WORLD"] = new UI::Button(
		this->_data, { 100.f, float(this->_data->winConfig.height - 70) }, { 250.f, 55.f },
		BLANK_COLOR, BLANK_COLOR, BLANK_COLOR,
		sf::Color(192, 192, 192), sf::Color::White, sf::Color::White,
		sf::Text("New World", this->_data->assets.GetFont("menu font"), 40U)
	);
}

void SavesState::HandleInput()
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
				this->_data->states.AddState(StateRef(new MainMenuState(this->_data)), true);
		}

		if (ev.type == sf::Event::MouseButtonPressed)
		{
			// checking every world plate for double click
			for (auto& plate : this->_worlds)
			{
				// if double click
				if (plate->GetClick(this->_data->window, sf::Mouse::Left) == UI::ClickType::DOUBLE)
				{
					// start game with selected world
					this->_data->states.AddState(StateRef(new GameState(this->_data, plate->getSettings())), true);
				}
			}
		}

		// scroller
		this->_scroller->handleInput(ev);
	}
}

void SavesState::Update(float deltaTime)
{
	if (this->_buttons["NEW_WORLD"]->isPressed(sf::Mouse::Button::Left))
	{
		this->_data->states.AddState(StateRef(new GenerationState(this->_data)), false);
	}

	// update world scroller
	this->_scroller->update(deltaTime);

	// updating worlds plates is going inside scroller class
	/*for (auto& plate : this->_worlds)
		plate->update(deltaTime);*/

	// updating buttons
	for (auto& [name, button] : this->_buttons)
		button->update(deltaTime);
}

void SavesState::Render() const
{
	this->_data->window.clear();

	// render background
	this->_data->window.draw(*this->_background);

	// render titles
	for (const auto& [name, title] : this->_titles)
		this->_data->window.draw(*title);

	// render scroller
	this->_scroller->render();

	this->_data->window.draw(*this->_downPanel);

	// rendering worlds plates is going inside scroller class
	/*for (const auto& plate : this->_worlds)
		plate->render();*/

	// rendering buttons
	for (const auto& [name, button] : this->_buttons)
		button->render();

	this->_data->window.display();
}

void SavesState::RefreshWorldsList()
{
	// clear all previous worlds plates
	for (size_t i = 0; i < this->_worlds.size(); i++)
		delete this->_worlds[i];
	this->_worlds.clear();

	namespace fs = std::filesystem;
	unsigned int i = 0;
	for (const auto& entry : fs::directory_iterator(WORLDS_DIR))
	{
		nlohmann::json json{};
		std::fstream info_file;
		info_file.open(entry.path().string() + "\\world_info.json", std::fstream::in); // world info file
		if (info_file.is_open())
		{
			info_file >> json;

			// getting world name from json
			Engine::WorldSaveSettings world_settings;
			json.at("name").get_to(world_settings.name);
			json.at("dir_path").get_to(world_settings.dir_path);

			sf::Vector2f plate_pos = 
			{
				float(this->_data->winConfig.width / 2 - this->_data->winConfig.width / 4),
				float(i * (WORLD_PLATE_HEIGHT + 10.f) + WORLD_PLATES_OFFSET) + WORLD_PLATE_OFFSET_FROM_TOP
			};

			// adding new world plate
			this->_worlds.push_back(new UI::WorldPlate(this->_data, world_settings,
				{ float(this->_data->winConfig.width / 2), WORLD_PLATE_HEIGHT },
				plate_pos,
				sf::Color(70, 63, 58)
			));
		}
		info_file.close();
		++i;
	}

	// reinit scroller
	this->_scroller->init();

	// if there are some worlds
	if (this->_worlds.size() > 0)
	{
		// delete NO_WORLDS_TITLE
		for (auto it = this->_titles.begin(); it != this->_titles.end(); ++it)
		{
			if (it->first == "NO_WORLDS_TITLE") // if found
			{
				delete it->second; // delete text ptr
				// erase it from map
				this->_titles.erase(it);
			}
		}
	}
}

void SavesState::Resume()
{
	// refresh worlds list
	this->RefreshWorldsList();
}
