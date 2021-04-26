#pragma once

#include "entities/Player.h"
#include "states/MainMenuState.h"
#include "engine/tiles/TileMap.h"
#include "engine/ui/PauseMenu.h"

class GameState 
	: public State
{
private:
	// data
	GameDataRef _data;
	bool _paused;

	// game objects
	Player* _player;

	PauseMenu* _pauseMenu;

	// camera
	sf::View _camera;

	// map
	Engine::TileMap* _map;

public:
	GameState(GameDataRef data);
	~GameState();

	void Init() override;

	void HandleInput() override;

	void Update(float deltaTime) override;
	void Render() const override;

	void Pause() override;
	void Resume() override;

};

