#include "TileMap.h"

namespace Engine
{
	TileMap::TileMap(GameDataRef data, unsigned int rows, unsigned int cols)
		:_data(data)
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
		// rendering all tiles
		for (const auto& row : _map)
		{
			for (const auto& tile : row)
				tile->render(_data->window);
		}
	}

}
