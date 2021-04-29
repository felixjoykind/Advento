#include "TileMap.h"

#include "engine/Physics.h"
#include <iostream>

namespace Engine
{
	TileMap::TileMap(GameDataRef data, const Entity& entity, unsigned int rows, unsigned int cols)
		:_data(data), _mapSize({ rows, cols }), _trackEntity(entity),
		nVisibleTiles({ _data->winConfig.width / 128, _data->winConfig.height / 128 }),
		_tilesRendered(0)
	{
		// creating tilemap
		for (unsigned int x = 0; x < rows; x++)
		{
			std::vector<Tile*> row; // new row
			for (unsigned int y = 0; y < cols; y++) // filling row
			{
				// TODO: remake system of choosing tile texture
				// generate or something
				const sf::Texture& texture = _data->assets.GetTexture("grass tile");
				row.push_back(new Tile(texture, { float(x * texture.getSize().x), float(y * texture.getSize().y) }));
			}
			_map.push_back(row); // adding row to map
		}
	}

	TileMap::~TileMap()
	{
		// deleting all tiles
		for (auto& row : _map)
		{
			for (auto& tile : row)
				delete tile;
		}
	}

	void TileMap::update(float deltaTime)
	{
		// updating all tiles
		for (auto& row : _map)
		{
			for (auto& tile : row)
				tile->update(deltaTime);
		}
	}

	void TileMap::render() const
	{
		// rendering tiles
		int fromX, toX;
		int fromY, toY;

		fromX = _trackEntity.getGridPosition(128).x - nVisibleTiles.x / 2 - 1;
		fromX = Physics::clamp<int>(0, _mapSize.x, fromX);

		fromY = _trackEntity.getGridPosition(128).y - nVisibleTiles.y / 2 - 1;
		fromY = Physics::clamp<int>(0, _mapSize.y, fromY);

		toX = _trackEntity.getGridPosition(128).x + nVisibleTiles.x / 2 + 2;
		toX = Physics::clamp<int>(0, _mapSize.x, toX);

		toY = _trackEntity.getGridPosition(128).y + nVisibleTiles.y / 2 + 2;
		toY = Physics::clamp<int>(0, _mapSize.y, toY);

		this->_tilesRendered = 0;
		for (size_t x = fromX; x < toX; x++)
		{
			for (size_t y = fromY; y < toY; y++)
			{
				_map[x][y]->render(_data->window);
				this->_tilesRendered++; // updating number of rendered tiles
			}
		}
	}

	// Returns the nimber of rendered tiles
	unsigned TileMap::tilesRendered() const
	{
		return this->_tilesRendered;
	}

}
