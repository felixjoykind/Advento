#pragma once

#include "Tile.h"
#include "gamedata/AssetManager.h"
#include "engine/defenitions/BASIC_WORLD_SETTINGS.h"

namespace Engine
{
	class Chunk
	{
	private:
		// data
		sf::Vector2u _position;
		char* _rawTiles;
		std::vector<Tile*> _tiles;
		
		mutable unsigned _tilesRendered;

	public:
		Chunk(const AssetManager& assets, sf::Vector2u pos, char* chunkMap);
		~Chunk();

		// getters
		sf::Vector2u getPosition() const;
		unsigned tilesRendered() const;

		void update(float deltaTime);
		void render(sf::RenderTarget& target) const;

		friend class TileMap;

	};
}
