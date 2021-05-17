#include "SavesState.h"

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

#include "engine/tiles/TileMap.h"
#include "engine/defenitions/PATH_DEFENTITIONS.h"

SavesState::SavesState(GameDataRef data)
	:_data(data)
{
}

SavesState::~SavesState()
{
}

void SavesState::Init()
{
	// load worlds
	namespace fs = std::filesystem;

	// if worlds dir doesn't exist
	if (!std::filesystem::exists(std::filesystem::path(WORLDS_DIR)))
	{
		std::filesystem::create_directories(WORLDS_DIR); // creating directories
	}

	unsigned int i = 0;
	for (const auto& entry : fs::directory_iterator(WORLDS_DIR))
	{
		nlohmann::json json{};
		std::fstream info_file;
		info_file.open(entry.path().string() + "\\world_info.json", std::fstream::in);
		if (info_file.is_open())
		{
			info_file >> json;

			// getting world name from json
			std::string world_name;
			json.at("name").get_to(world_name);

			// adding new world plate
			this->_worlds.push_back(new UI::WorldPlate(this->_data, world_name, 
				{ float(this->_data->winConfig.width / 2), 100.f },
				{ 
					float(this->_data->winConfig.width / 2 - this->_data->winConfig.width / 4) ,
					float(i * 100.f + 20.f)
				},
				sf::Color(195, 195, 195)));
		}
		info_file.close();
		++i;
	}

	// init buttons
	this->_buttons["NEW_WORLD"] = new UI::Button(
		this->_data, { 100.f, float(this->_data->winConfig.height - 70) }, { 250.f, 55.f },
		sf::Color(105, 105, 105), sf::Color(115, 115, 115), sf::Color(125, 125, 125),
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
				this->_data->states.RemoveState();
		}
	}
}

void SavesState::Update(float deltaTime)
{
	if (this->_buttons["NEW_WORLD"]->isPressed(sf::Mouse::Button::Left))
	{
		Engine::TileMap map(this->_data, 256, 256);
		map.generate({ "test_seed", 256, 256, 0.4f, 4, 3, 5 });
		map.save_to({ "test_world", WORLDS_DIR + std::string("\\test_world") });

		this->_data->states.AddState(StateRef(new GenerationState(this->_data)), true);
	}

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

	// rendering worlds plates
	for (const auto& plate : this->_worlds)
		plate->render();

	// rendering buttons
	for (const auto& [name, button] : this->_buttons)
		button->render();

	this->_data->window.display();
}
