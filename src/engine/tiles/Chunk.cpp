#include "Chunk.h"

#define BORDER_THICKNESS 5.f

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

		// adding borders if necessary
		if (pos.x == 0)
		{
			// new border
			sf::RectangleShape* border = new sf::RectangleShape(
				{ BORDER_THICKNESS, CHUNK_SIZE_PIXEL }
			);
			border->setPosition(
				{ -BORDER_THICKNESS, pos.y * CHUNK_SIZE_PIXEL }
			);
			border->setFillColor(sf::Color::Red);

			// add new border
			this->_borders.push_back(border);
		}
		else if (pos.x == BASIC_WORLD_SIZE_X / CHUNK_SIZE - 1)
		{
			// new border
			sf::RectangleShape* border = new sf::RectangleShape(
				{ BORDER_THICKNESS, CHUNK_SIZE_PIXEL }
			);
			border->setPosition(
				{ pos.x * CHUNK_SIZE_PIXEL + CHUNK_SIZE_PIXEL, pos.y * CHUNK_SIZE_PIXEL }
			);
			border->setFillColor(sf::Color::Red);

			// add new border
			this->_borders.push_back(border);
		}

		if (pos.y == 0)
		{
			// new border
			sf::RectangleShape* border = new sf::RectangleShape(
				{ CHUNK_SIZE_PIXEL, BORDER_THICKNESS }
			);
			border->setPosition(
				{ pos.x * CHUNK_SIZE_PIXEL, -BORDER_THICKNESS }
			);
			border->setFillColor(sf::Color::Red);

			// add new border
			this->_borders.push_back(border);
		}
		else if (pos.y == BASIC_WORLD_SIZE_Y / CHUNK_SIZE - 1)
		{
			// new border
			sf::RectangleShape* border = new sf::RectangleShape(
				{ CHUNK_SIZE_PIXEL, BORDER_THICKNESS }
			);
			border->setPosition(
				{ pos.x * CHUNK_SIZE_PIXEL, pos.y * CHUNK_SIZE_PIXEL + CHUNK_SIZE_PIXEL }
			);
			border->setFillColor(sf::Color::Red);

			// add new border
			this->_borders.push_back(border);
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

		// render tiles
		for (size_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (size_t y = 0; y < CHUNK_SIZE; y++)
			{
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

		// render borders
		for (const auto& border : this->_borders)
		{
			target.draw(*border);
		}
	}
}
