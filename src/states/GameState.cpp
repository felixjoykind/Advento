#include "GameState.h"

#include <iostream>

GameState::GameState(GameDataRef data)
	:_data(data), _paused(false),
	_camera(sf::View({ 0.f, 0.f }, { (float)_data->winConfig.width, (float)_data->winConfig.height })),
	_player(new Player(_data, { float(_data->winConfig.width / 2), float(_data->winConfig.height / 2) })),
	_map(new Engine::TileMap(_data, *_player, 16, 16)),
	_pauseMenu(new PauseMenu(_data)), _debugInfo(new Engine::DebugInfo(_data, { *_player, *_map }))
{
}

GameState::~GameState()
{
	delete this->_player;
	delete this->_pauseMenu;
	delete this->_map;
	delete this->_debugInfo;
}

void GameState::Init()
{
	// state init
}

void GameState::HandleInput()
{
	sf::Event ev;
	while (_data->window.pollEvent(ev))
	{
		if (ev.type == sf::Event::Closed)
		{
			_data->window.close();
		}

		// keyboard events
		if (ev.type == sf::Event::KeyPressed)
		{
			if (ev.key.code == sf::Keyboard::Escape)
			{
				if (this->_paused)
				{
					// unpausing state
					this->Resume();
				}
				else
				{
					// pausing state
					this->Pause();
				}
			}
			else if (ev.key.code == sf::Keyboard::F3)
			{
				this->_debugInfo->setActive(!_debugInfo->isActive());
				std::cout << "F3 : " << _debugInfo->isActive() << std::endl;
			}
		}
	}
}

void GameState::Update(float deltaTime)
{
	// if paused
	if (this->_paused)
	{
		// check if still paused
		if (this->_pauseMenu->isActive())
		{
			this->_pauseMenu->update(deltaTime);
		}
		else // resume
		{
			this->Resume();
		}
		return;
	}

	// updating player
	this->_player->update(deltaTime);

	// updating camera
	this->_camera.setCenter(_player->getPosition());

	// updating map
	this->_map->update(deltaTime);

	if (this->_debugInfo->isActive())
	{
		// update debug info
		this->_debugInfo->update(deltaTime);
	}
}

void GameState::Render() const
{
	// rednering things with the right view
	_data->window.clear();
	_data->window.setView(_camera);

	// map
	this->_map->render();

	// rendering player
	this->_player->render();

	this->_data->window.setView(_data->window.getDefaultView()); // setting default view
	// if paused
	if (this->_paused)
	{
		// render pause menu
		this->_pauseMenu->render();
	}

	// if showing debug info
	if (this->_debugInfo->isActive())
	{
		// rednering debug information
		this->_debugInfo->render(_data->window);
	}

	_data->window.display();
}

void GameState::Pause()
{
	this->_pauseMenu->setActive(true);
	this->_paused = true;
}

void GameState::Resume()
{
	this->_paused = false;
}
