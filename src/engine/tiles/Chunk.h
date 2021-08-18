#pragma once

#include "Tile.h"
#include "entities/trees/TreesManager.h"
#include "gamedata/AssetManager.h"
#include "engine/defenitions/BASIC_WORLD_SETTINGS.h"
#include "engine/generator/MapGenerator.h"

namespace Engine
{
	class Chunk
	{
	private:
		// data
		sf::Vector2u _position;
		char** _rawTiles;
		std::vector<Tile*> _tiles;

		// trees in chunk
		std::vector<std::unique_ptr<Advento::Tree>> _trees;

		std::vector<sf::RectangleShape*> _borders;
		
		mutable unsigned _tilesRendered;

	public:
		Chunk(const AssetManager& assets, TreesManager& trees_manager, sf::Vector2u pos, char** chunkMap);
		~Chunk();

		// Returns chunk position in chunks grid
		sf::Vector2u getPosition() const;

		// Returns the number of rendered tiles
		unsigned tilesRendered() const;

		void update(float deltaTime);
		void render(sf::RenderTarget& target) const;

		friend class TileMap;

	};
}
