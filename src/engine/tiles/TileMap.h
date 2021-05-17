#pragma once

#include "Game.h"
#include "Tile.h"
#include "engine/ecs/Entity.h"
#include "engine/generator/MapGenerator.h"
#include <nlohmann/json.hpp>

namespace Engine
{
	struct WorldSaveSettings
	{
		std::string name;
		std::string dir_path;
	};

	class Tile;
	class TileMap
	{
	private:
		// data
		GameDataRef _data;
		sf::Vector2u _mapSize;
		std::vector<std::vector<Tile*>> _map; // map
		char** _rawMap; // raw map

		// tiles rendering data
		sf::Vector2u nVisibleTiles;
		mutable unsigned _tilesRendered;

	public:
		TileMap(GameDataRef data, unsigned int rows, unsigned int cols);
		~TileMap();

		// Returns the number of rendered tiles
		unsigned tilesRendered() const;

		// Returns the size of the map
		sf::Vector2u getSize() const;

		// Generates new map based on settings
		void generate(GenerationSettings settings);

		// json formaters
		void save_to(const WorldSaveSettings settings) const;
		void load_from(const std::string filepath);

		// basic functions
		void update(float deltaTime);
		void render(const Entity& trackEntity) const;

	};
}
