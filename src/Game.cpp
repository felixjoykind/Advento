#include "Game.h"

#include <fstream>
#include <filesystem>
#include "states/MainMenuState.h"

// config files paths
#define WINDOW_CFG_FULL_FILEPATH "C:\\Users\\User\\AppData\\Roaming\\.advento\\cfg\\window_cfg.ini"
#define WINDOW_CFG_DIR_FILEPATH "C:\\Users\\User\\AppData\\Roaming\\.advento\\cfg" // directories path (for creating)

Game::Game()
{
	this->read_cfg(); // reading window config
	this->_data->window.create(sf::VideoMode(_data->winConfig.width, _data->winConfig.height),
		_data->winConfig.title,  _data->winConfig.fullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar | sf::Style::Close);

	// loading textures
	this->_data->assets.AddTexture("player", "./assets/sprites/player/player.png");
	this->_data->assets.AddTexture("grass tile", "./assets/sprites/tiles/grass.png");
	this->_data->assets.AddTexture("water tile", "./assets/sprites/tiles/ocean.png");

	// loading animations
	this->_data->assets.AddTexture("player anim", "./assets/sprites/player/player_anim.png");

	// loading fonts
	this->_data->assets.AddFont("menu font", "./assets/fonts/Jura-Regular.ttf");

	this->_data->states.AddState(StateRef(new MainMenuState(this->_data)), false);

	this->_data->window.setFramerateLimit(60);
}

Game::~Game()
{ }

void Game::Run()
{
	this->_deltaClock.restart();
	this->_fpsClock.restart();
	while (_data->window.isOpen())
	{
		_data->states.ProcessStateChanges();
		_data->states.GetActiveState()->HandleInput();
		_data->states.GetActiveState()->Update(_deltaClock.restart().asSeconds());
		_data->states.GetActiveState()->Render();

		// updating fps
		_data->winConfig.fps = 1.0f / this->_fpsClock.restart().asSeconds();
	}
}

void Game::read_cfg()
{
	std::fstream cfg_file;
	std::filesystem::create_directories(WINDOW_CFG_DIR_FILEPATH); // creating directories
	cfg_file.open(WINDOW_CFG_FULL_FILEPATH, std::fstream::in | std::fstream::out | std::fstream::app);
	if (cfg_file.peek() == std::fstream::traits_type::eof()) // if file doesn't exist
	{
		// creating cfg file and writing default values
		cfg_file.seekg(0, std::fstream::beg); // going to beginning
		cfg_file << "Advento" << std::endl;
		cfg_file << 1920 << " " << 1080 << std::endl;
		cfg_file << 1;
	}

	if (cfg_file.is_open())
	{
		// reading window config
		cfg_file.seekg(0, std::fstream::beg); // going to beginning
		std::getline(cfg_file, _data->winConfig.title);
		cfg_file >> _data->winConfig.width >> _data->winConfig.height;
		cfg_file >> _data->winConfig.fullscreen;
	}

	cfg_file.close();
}