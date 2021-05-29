#include "Chunk.h"

#include <iostream>

namespace Engine
{
	Chunk::Chunk(const AssetManager& assets, sf::Vector2u pos, char* chunkMap)
		:_position(pos), _rawTiles(chunkMap)
	{
		// filling map based on input
		for (size_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (size_t y = 0; y < CHUNK_SIZE; y++)
			{
				std::string texture_name; // texture name
				// choosing texture name based on input
				switch (this->_rawTiles[y * CHUNK_SIZE + x])
				{
				case 'g':
					texture_name = "grass tile";
					break;
				case '.':
					texture_name = "water tile";
					break;

				}

				// calculating tile position
				unsigned int chunk_to_map_x = pos.x == 0 ? x : x + CHUNK_SIZE * pos.x;
				unsigned int chunk_to_map_y = pos.y == 0 ? y : y + CHUNK_SIZE * pos.y;

				sf::Vector2f tile_pos =
				{
					float(chunk_to_map_x) * TILE_SIZE,
					float(chunk_to_map_y) * TILE_SIZE
				};
				this->_tiles.push_back(new Tile(assets.GetTexture(texture_name), tile_pos)); // adding new tile
				//{ float((x + 1) * pos.x) * TILE_SIZE, float((y + 1) * pos.y) * TILE_SIZE }
			}
		}
	}

	Chunk::~Chunk()
	{
		// clear cnunk map data
		delete[] this->_rawTiles;
		this->_tiles.clear();
	}

	sf::Vector2u Chunk::getPosition() const
	{
		return this->_position;
	}

	void Chunk::update(float deltaTime)
	{
		for (size_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (size_t y = 0; y < CHUNK_SIZE; y++)
			{
				this->_tiles[y * CHUNK_SIZE + x]->update(deltaTime);
			}
		}
	}

	void Chunk::render(sf::RenderTarget& target) const
	{
		for (size_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (size_t y = 0; y < CHUNK_SIZE; y++)
			{
				this->_tiles[y * CHUNK_SIZE + x]->render(target);
			}
		}
	}
}
