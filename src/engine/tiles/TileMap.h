#pragma once

#include "Game.h"
#include "Tile.h"
#include <entities/Entity.h>

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

		unsigned nVisibleTiles; // TODO: make x and y variants

		mutable int fromX, toX;
		mutable int fromY, toY;

	public:
		TileMap(GameDataRef data, const Entity& entity, unsigned int rows, unsigned int cols);
		~TileMap();

		// basic functions
		void update(float deltaTime);
		void render() const;

	};
}
