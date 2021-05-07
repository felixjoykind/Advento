#pragma once

#include "Game.h"
#include "Tile.h"
#include "engine/ecs/Entity.h"
#include "engine/generator/MapGenerator.h"

namespace Engine
{
	class Tile;
	class TileMap
	{
	private:
		// data
		GameDataRef _data;
		sf::Vector2u _mapSize;
		std::vector<std::vector<Tile*>> _map; // map
		const Entity& _trackEntity;

		// tiles rendering data
		sf::Vector2u nVisibleTiles;
		mutable unsigned _tilesRendered;

	public:
		TileMap(GameDataRef data, const Entity& entity, unsigned int rows, unsigned int cols);
		~TileMap();

		// Returns the number of rendered tiles
		unsigned tilesRendered() const;

		// Returns the size of the map
		sf::Vector2u getSize() const;

		// Generates new map based on settings
		void generate(GenerationSettings settings);

		// basic functions
		void update(float deltaTime);
		void render() const;

	};
}
