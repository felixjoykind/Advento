#include "GameState.h"

GameState::GameState(GameDataRef data)
	:_data(data), _paused(false),
	_camera(sf::View({ 0.f, 0.f }, { (float)_data->winConfig.width, (float)_data->winConfig.height })),
	_player(new Player(_data, { float(_data->winConfig.width / 2), float(_data->winConfig.height / 2) })),
	_pauseMenu(new PauseMenu(_data)),
	_map(new Engine::TileMap(_data, *_player, 16, 16))
{
}

GameState::~GameState()
{
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
		}
	}
}

void GameState::Update(float deltaTime)
{
	if (this->_paused)
	{
		if (this->_pauseMenu->isActive())
		{
			this->_pauseMenu->update(deltaTime);
		}
		else
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
}

void GameState::Render() const
{
	_data->window.clear();
	_data->window.setView(_camera);

	// map
	this->_map->render();

	// rendering player
	this->_player->render();

	if (this->_paused)
	{
		this->_data->window.setView(_data->window.getDefaultView());
		this->_pauseMenu->render();
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
