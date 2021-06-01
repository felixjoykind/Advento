#include "SavesState.h"

#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "engine/defenitions/UI_DEFENITIONS.h"
#include "engine/defenitions/PATH_DEFENITIONS.h"
#include "engine/defenitions/COLORS.h"

#include "gamedata/InputManager.h"
#include "engine/SpriteManipulator.h"

#include "engine/tiles/TileMap.h"

#include "MainMenuState.h"
#include "states/GameState.h"

SavesState::SavesState(GameDataRef data)
	:_data(data), _background(new sf::Sprite(this->_data->assets.GetTexture("saves menu background"))),
	_scroller(new UI::Scroller<UI::WorldPlate>(
		_data, { 15.f, 400.f }, 
		{ float(_data->winConfig.width / 2 + _data->winConfig.width / 4) + 10.f, 20.f },
		{ 20.f, float(_data->winConfig.height) },
		_worlds)
	)
{
}

SavesState::~SavesState()
{
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

	// init buttons
	this->_buttons["NEW_WORLD"] = new UI::Button(
		this->_data, { 100.f, float(this->_data->winConfig.height - 70) }, { 250.f, 55.f },
		BLANK_COLOR, BLANK_COLOR, BLANK_COLOR,
		sf::Color(192, 192, 192), sf::Color::White, sf::Color::White,
		sf::Text("New World", this->_data->assets.GetFont("menu font"), 40)
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

	// updating worlds plates
	for (auto& plate : this->_worlds)
		plate->update(deltaTime);

	// updating buttons
	for (auto& [name, button] : this->_buttons)
		button->update(deltaTime);
}

void SavesState::Render() const
{
	this->_data->window.clear();

	// render background
	this->_data->window.draw(*this->_background);

	// render scroller
	this->_scroller->render();

	// rendering worlds plates
	for (const auto& plate : this->_worlds)
		plate->render();

	// rendering buttons
	for (const auto& [name, button] : this->_buttons)
		button->render();

	this->_data->window.display();
}

void SavesState::RefreshWorldsList()
{
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

			// adding new world plate
			this->_worlds.push_back(new UI::WorldPlate(this->_data, world_settings,
				{ float(this->_data->winConfig.width / 2), 100.f },
				{
					float(this->_data->winConfig.width / 2 - this->_data->winConfig.width / 4),
					float(i * 110.f + WORLD_PLATES_OFFSET)
				},
				sf::Color(70, 63, 58)
			));
		}
		info_file.close();
		++i;
	}

	// reinit scroller
	this->_scroller->init();
}

void SavesState::Resume()
{
	// refresh worlds list
	this->RefreshWorldsList();
}
