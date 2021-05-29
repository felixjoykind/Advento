#include "GameState.h"

#include "entities/Player.h"
#include <fstream>
#include "engine/defenitions/PATH_DEFENITIONS.h"

GameState::GameState(GameDataRef data, Engine::WorldSaveSettings world_settings)
	:_data(data), _paused(false),
	_manager(new Engine::EntityManager(_data)),
	_player(_manager->createEntity<Player, sf::Vector2f>({ 0.f, 0.f })),
	_camera(sf::View({ 0.f, 0.f }, { float(_data->winConfig.width), float(_data->winConfig.height) })),
	_pauseMenu(new PauseMenu(_data))
{
	this->_map = new Engine::TileMap(this->_data, 256, 256, &this->_player);
	this->_map->load_from(world_settings.dir_path);

	this->_debugInfo = new Engine::DebugInfo(_data, { _player, *_map });

	// setting player position to the center of the map
	this->_player.getComponent<Engine::PositionComponent>().setPosition(
		float(_map->getSize().x / 2 * 128),
		float(_map->getSize().y / 2 * 128)
	);
}

GameState::~GameState()
{
	// deleting pointers
	delete this->_manager;
	delete this->_map;
	delete this->_pauseMenu;
	delete this->_debugInfo;
}

void GameState::Init()
{
	// state init
	
	// view
	this->_camera.setCenter(this->_player.getComponent<Engine::PositionComponent>().getPosition());
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
				// showing all debug info
				this->_debugInfo->setActive(!_debugInfo->isActive());
				for (const auto& e : _manager->getEntities())
				{
					// if entity has hitbox
					if (e->hasComponent<Engine::HitboxComponent>())
					{
						// enable/disable hitbox
						auto& hitbox = e->getComponent<Engine::HitboxComponent>();
						hitbox.setVisible(!hitbox.getVisible());
					}
				}
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
	this->_player.update(deltaTime);

	// updating camera (moving with player)
	this->_camera.setCenter(this->_player.getComponent<Engine::PositionComponent>().getPosition());

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
	this->_player.render();

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
		this->_debugInfo->render();
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
