#include "GameState.h"

#include "entities/Player.h"
#include <fstream>
#include "engine/defenitions/PATH_DEFENITIONS.h"

GameState::GameState(GameDataRef data, Engine::WorldSaveSettings world_settings)
	:_data(data), _paused(false),
	_manager(new Engine::EntityManager(_data)),
	_player(_manager->createEntity<Player, sf::Vector2f>({ 0.f, 0.f })),
	_map(new Engine::TileMap(this->_data, 256, 256, &this->_player)),
	_playerInventory(new UI::PlayerInventory(_data, _player, this->_map->getWorldItemsManager())),
	_pauseMenu(new PauseMenu(_data)),
	_debugInfo(new Engine::DebugInfo(_data, { _player, *_map }))
{
	this->_map->load_from(world_settings.dir_path);
}

GameState::~GameState()
{
	// deleting pointers
	delete this->_manager;
	delete this->_playerInventory;
	delete this->_map;
	delete this->_pauseMenu;
	delete this->_debugInfo;
}

void GameState::Init()
{
	// state init
	this->_data->gameCamera = sf::View({ 0.f, 0.f }, { float(_data->winConfig.width), float(_data->winConfig.height) });

	// setting player position to the center of the map
	this->_player.getComponent<Engine::PositionComponent>().setPosition(
		float(_map->getSize().x / 2) * TILE_SIZE,
		float(_map->getSize().y / 2) * TILE_SIZE
	);
	
	// view
	this->_data->gameCamera.setCenter(this->_player.getComponent<Engine::PositionComponent>().getPosition());
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
				{ // unpausing state
					this->Resume();
				}
				else
				{ // pausing state
					this->Pause();
				}
			}
			else if (ev.key.code == sf::Keyboard::F3)
			{ // showing all debug info
				this->_debugInfo->setActive(!_debugInfo->isActive());
				for (const auto& e : _manager->getEntities())
				{
					// if entity has hitbox
					if (e->hasComponent<Engine::HitboxComponent>())
					{ // enable/disable hitbox
						auto& hitbox = e->getComponent<Engine::HitboxComponent>();
						hitbox.setVisible(!hitbox.getVisible());
					}
				}
			}
			else if (ev.key.code == sf::Keyboard::E)
			{ // open inventory
				if (this->_playerInventory->isActive())
					this->_playerInventory->close();
				else
					this->_playerInventory->open();
			}
		}

		this->_playerInventory->handleInput(ev);
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
		else
		{ // resume
			this->Resume();
		}
		return;
	}

	// clamp player position
	auto& player_pos = this->_player.getComponent<Engine::PositionComponent>();

	player_pos.setX(Math::clamp<float>(0.f, this->_map->getSize().x * TILE_SIZE - PLAYER_HITBOX_SIZE_X, player_pos.getX()));
	player_pos.setY(Math::clamp<float>(0.f, this->_map->getSize().y * TILE_SIZE - PLAYER_HITBOX_SIZE_Y, player_pos.getY()));

	// updating player
	if (this->_playerInventory->isActive() == false)
		this->_player.handleInput();
	this->_player.update(deltaTime);

	// updating camera (moving with player)
	this->_data->gameCamera.setCenter(this->_player.getComponent<Engine::PositionComponent>().getPosition());

	// updating map
	this->_map->update(deltaTime);

	if (this->_debugInfo->isActive())
	{ // update debug info
		this->_debugInfo->update(deltaTime);
	}

	if (this->_playerInventory->isActive())
	{ // update player inventory
		this->_playerInventory->update(deltaTime);
	}
}

void GameState::Render() const
{
	// rednering things with the right view
	_data->window.clear();
	_data->window.setView(this->_data->gameCamera);

	// map
	this->_map->render();

	// rendering player
	this->_player.render();

	this->_data->window.setView(_data->window.getDefaultView()); // setting default view

	if (this->_paused)
	{ // render pause menu
		this->_pauseMenu->render();
	}
	else
	{
		if (this->_debugInfo->isActive())
		{ // rednering debug information
			this->_debugInfo->render();
		}

		// render player inventory
		this->_playerInventory->render();
	}

	_data->window.display();
}

void GameState::Pause()
{
	this->_playerInventory->setActive(false);
	this->_pauseMenu->setActive(true);
	this->_paused = true;
}

void GameState::Resume()
{
	this->_paused = false;
	this->_playerInventory->open();
}
