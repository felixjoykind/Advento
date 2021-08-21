#include "Chunk.h"

#define BORDER_THICKNESS 5.f

namespace Engine
{
	Chunk::Chunk(const AssetManager& assets, TreesManager& trees_manager, sf::Vector2u pos, char** chunkMap)
		:_position(pos), _rawTiles(chunkMap), _tilesRendered(0)
	{
		// filling map based on input
		for (size_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (size_t y = 0; y < CHUNK_SIZE; y++)
			{
				std::string block_texture_name; // block texture name
				std::string tree_texture_name;

				// choosing block and tree texture name based on input
				switch (this->_rawTiles[y * CHUNK_SIZE + x][0])
				{
				case 'g':
					block_texture_name = "grass tile";
					break;
				case 'w':
					block_texture_name = "water tile";
					break;

				}

				//std::vector<Tile*> tile_layer;

				// calculating tile position
				unsigned int chunk_to_map_x = pos.x == 0 ? x : x + CHUNK_SIZE * pos.x;
				unsigned int chunk_to_map_y = pos.y == 0 ? y : y + CHUNK_SIZE * pos.y;

				sf::Vector2f tile_pos =
				{
					float(chunk_to_map_x) * TILE_SIZE,
					float(chunk_to_map_y) * TILE_SIZE
				};

				switch (this->_rawTiles[y * CHUNK_SIZE + x][1])
				{
				case 't':
					//tree_texture_name = "tree tile";
					this->_trees.push_back(trees_manager.addTree(tile_pos).get());
					break;
				case '.':
					//tree_texture_name = "empty tile";
					break;

				}

				// push tiles to layer
				//tile_layer.push_back(new Tile(assets.GetTexture(block_texture_name), tile_pos)); // adding new tile
				//tile_layer.push_back(new Tile(assets.GetTexture(tree_texture_name), tile_pos)); // adding new tile

				// push layer
				//this->_tiles.push_back(tile_layer); // adding new tile
				this->_tiles.push_back(new Tile(assets.GetTexture(block_texture_name), tile_pos)); // adding new tile
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

		for (size_t i = 0; i < this->_tiles.size(); i++)
			delete this->_tiles[i];
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
				//this->_tiles[y * CHUNK_SIZE + x][1]->update(deltaTime);
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
				auto& target_center = target.getView().getCenter();

				auto window_bounds = sf::FloatRect(
					{
						float(target_center.x - target_size.x / 2),
						float(target_center.y - target_size.y / 2)
					},
					{ float(target_size.x), float(target_size.y) }
				);

				if (tile->isInBounds(window_bounds))
				{
					tile->render(target);
					this->_tilesRendered++;
				}

				/*for (size_t z = 0; z < MAP_LAYER_DEPTH; z++)
				{
					if (z == 1)
					{
						if (this->_rawTiles[y * CHUNK_SIZE + x][z] == '.')
							continue;
					}

					auto& tile = this->_tiles[y * CHUNK_SIZE + x][z];
					auto target_size = target.getSize();
					auto target_center = target.getView().getCenter();

					auto window_bounds = sf::FloatRect(
						{
							float(target_center.x - target_size.x / 2),
							float(target_center.y - target_size.y / 2)
						},
						{ float(target_size.x), float(target_size.y) }
					);

					if (tile->isInBounds(window_bounds))
					{
						tile->render(target);
						this->_tilesRendered++;
					}
				}*/
			}
		}

		// render borders
		for (const auto& border : this->_borders)
		{
			target.draw(*border);
		}
	}
}
