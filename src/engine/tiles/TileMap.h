#pragma once

#include "Game.h"
#include "Tile.h"

namespace Engine
{
	class Tile;
	class TileMap
	{
	private:
		GameDataRef _data;
		std::vector<std::vector<Tile*>> _map;

	public:
		TileMap(GameDataRef data, unsigned int rows, unsigned int cols);
		~TileMap();

		// basic functions
		void update(float deltaTime);
		void render() const;

	};
}
