#include "Chunk.h"

namespace Engine
{
	Chunk::Chunk(const AssetManager& assets, sf::Vector2u pos, char* chunkMap)
		:_position(pos), _rawTiles(chunkMap), _tilesRendered(0)
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
			}
		}
	}

	Chunk::~Chunk()
	{
		// clear cnunk map data
		delete[] this->_rawTiles;
		this->_tiles.clear();
	}

	sf::Vector2u Chunk::getPosition() const { return this->_position; }
	unsigned Chunk::tilesRendered() const { return this->_tilesRendered; }

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
		// reset number of rendered tiles
		this->_tilesRendered = 0;

		for (size_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (size_t y = 0; y < CHUNK_SIZE; y++)
			{
				// TODO: refactor
				auto& tile = this->_tiles[y * CHUNK_SIZE + x];
				auto target_size = target.getSize();
				auto target_center = target.getView().getCenter();

				auto window_bounds = sf::FloatRect(
					{ 
						float(target_center.x -  target_size.x / 2),
						float(target_center.y - target_size.y / 2)
					},
					{ float(target_size.x), float(target_size.y) }
				);

				if (tile->isInBounds(window_bounds))
				{
					tile->render(target);
					this->_tilesRendered++;
				}
			}
		}
	}
}
