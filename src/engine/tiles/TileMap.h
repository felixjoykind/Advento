#pragma once

#include "Game.h"
#include "Tile.h"
#include "engine/ecs/Entity.h"

namespace Engine
{
	class Tile;
	class TileMap
	{
	private:
		// data
		GameDataRef _data;
		sf::Vector2u _mapSize;
		std::vector<std::vector<Tile*>> _map;
		const Entity& _trackEntity;

		// tiles reendering data
		sf::Vector2u nVisibleTiles;
		mutable unsigned _tilesRendered;

	public:
		TileMap(GameDataRef data, const Entity& entity, unsigned int rows, unsigned int cols);
		~TileMap();

		// getters
		unsigned tilesRendered() const;

		// basic functions
		void update(float deltaTime);
		void render() const;

	};
}
