#include "Game.h"

#include <fstream>
#include "states/MainMenuState.h"
#include "engine/defenitions/PATH_DEFENITIONS.h"

Game::Game()
{
	this->read_cfg(); // reading window config
	this->_data->window.create(sf::VideoMode(_data->winConfig.width, _data->winConfig.height),
		_data->winConfig.title,  _data->winConfig.fullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar | sf::Style::Close);

	// loading textures
	this->_data->assets.AddTexture("grass tile", "./assets/sprites/tiles/grass.png");
	this->_data->assets.AddTexture("water tile", "./assets/sprites/tiles/ocean.png");
	this->_data->assets.AddTexture("tree tile", "./assets/sprites/tiles/tree.png");
	this->_data->assets.AddTexture("empty tile", "./assets/sprites/tiles/empty.png");
	this->_data->assets.AddTexture("main menu background", "./assets/sprites/menus and panels/main menu/background.png");
	this->_data->assets.AddTexture("saves menu background", "./assets/sprites/menus and panels/saves menu/background.jpg");
	this->_data->assets.AddTexture("generation menu background", "./assets/sprites/menus and panels/generation menu/background.jpg");

	// loading items
	this->_data->assets.AddTexture("inventory", "./assets/sprites/items/inventory.png"); // inventory
	this->_data->assets.AddTexture("inventory hud", "./assets/sprites/items/inventory_hud.png"); // inventory hud
	this->_data->assets.AddTexture("empty item", "./assets/sprites/items/empty_item.png");
	this->_data->assets.AddTexture("stick item", "./assets/sprites/items/stick.png");

	// loading animations
	this->_data->assets.AddTexture("player anim", "./assets/sprites/player/player_anim.png");

	// loading fonts
	this->_data->assets.AddFont("menu font", "./assets/fonts/Jura-Regular.ttf");
	this->_data->assets.AddFont("menu title font", "./assets/fonts/Comfortaa-Light.ttf");
	this->_data->assets.AddFont("SegoeUI Regular", "./assets/fonts/SegoeUI.ttf");
	this->_data->assets.AddFont("SegoeUI Semibold", "./assets/fonts/SegoeUI-SemiBold.ttf");

	// starting game with main menu state
	this->_data->states.AddState(StateRef(new MainMenuState(this->_data)), false);

	this->_data->window.setFramerateLimit(120);
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
	if (!std::filesystem::exists(std::filesystem::path(CFG_DIR)))
		std::filesystem::create_directories(CFG_DIR); // creating directories

	std::fstream cfg_file;
	cfg_file.open(WINDOW_CFG_FILEPATH, std::fstream::in | std::fstream::out | std::fstream::app);
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