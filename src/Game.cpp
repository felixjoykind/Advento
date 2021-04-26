#include "Game.h"
#include <fstream>

#include "states/MainMenuState.h"

Game::Game()
{
	this->read_cfg();
	this->_data->window.create(sf::VideoMode(_data->winConfig.width, _data->winConfig.height), 
		_data->winConfig.title, sf::Style::Titlebar | sf::Style::Close);

	// loading textures
	this->_data->assets.AddTexture("player", "./assets/sprites/player/player.png");
	this->_data->assets.AddTexture("grass tile", "./assets/sprites/tiles/grass.png");
	this->_data->assets.AddTexture("ocean tile", "./assets/sprites/tiles/ocean.png");

	// loading animations
	this->_data->assets.AddTexture("player anim", "./assets/sprites/player/player_anim.png");

	// loading fonts
	this->_data->assets.AddFont("menu font", "./assets/fonts/Jura-Regular.ttf");

	this->_data->states.AddState(StateRef(new MainMenuState(this->_data)), false);

	this->_data->window.setFramerateLimit(60);
}

Game::~Game()
{ }

void Game::run()
{
	this->_deltaClock.restart();
	while (_data->window.isOpen())
	{
		_data->states.ProcessStateChanges();
		_data->states.GetActiveState()->HandleInput();
		_data->states.GetActiveState()->Update(_deltaClock.restart().asSeconds());
		_data->states.GetActiveState()->Render();
	}
}


void Game::read_cfg()
{
	std::fstream cfg_file;
	cfg_file.open("./window_cfg.ini", std::fstream::in | std::fstream::out | std::fstream::app);
	if (cfg_file.peek() == std::fstream::traits_type::eof()) // if file doesn't exist
	{
		// creating cfg file and writing default values
		cfg_file.seekg(0, std::fstream::beg); // going to beginning
		cfg_file << "Advento" << std::endl;
		cfg_file << 1290 << " " << 960;
	}

	if (cfg_file.is_open())
	{
		// reading window config
		cfg_file.seekg(0, std::fstream::beg); // going to beginning
		std::getline(cfg_file, _data->winConfig.title);
		cfg_file >> _data->winConfig.width >> _data->winConfig.height;
	}

	cfg_file.close();
}