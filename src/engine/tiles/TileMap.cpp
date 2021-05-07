#include "TileMap.h"

#include "engine/Physics.h"
#include "engine/ecs/components/PositionComponent.h"

namespace Engine
{
	TileMap::TileMap(GameDataRef data, const Entity& entity, unsigned int rows, unsigned int cols)
		:_data(data), _mapSize({ rows, cols }), _trackEntity(entity),
		nVisibleTiles({ _data->winConfig.width / 128, _data->winConfig.height / 128 }),
		_tilesRendered(0)
	{
	}

	TileMap::~TileMap()
	{
		// deleting all tiles
		for (auto& row : this->_map)
		{
			for (auto& tile : row)
				delete tile;
		}
		this->_map.clear();
	}

	void TileMap::generate(GenerationSettings settings)
	{
		// getting char map and clearing previous
		auto gen_map = std::move(MapGenerator::Generate(settings));
		this->_map.clear();
		
		// initializing map
		this->_map = std::vector<std::vector<Tile*>>(settings.width);
		for (auto& row : _map)
			row = std::vector<Tile*>(settings.height);

		// filling map based on input
		for (size_t x = 0; x < settings.width; x++)
		{
			for (size_t y = 0; y < settings.height; y++)
			{
				std::string texture_name; // texture name
				// choosing texture name based on input
				switch (gen_map[x][y])
				{
				case 'g':
					texture_name = "grass tile";
					break;
				case '.':
					texture_name = "water tile";
					break;
				}
				// adding new tile
				this->_map[x][y] = new Tile(_data->assets.GetTexture(texture_name), { (float)x * 128.f, (float)y * 128.f });
			}
		}
	}

	sf::Vector2u TileMap::getSize() const
	{
		return this->_mapSize;
	}

	unsigned TileMap::tilesRendered() const
	{
		return this->_tilesRendered;
	}

	void TileMap::update(float deltaTime)
	{
		// update tile map
	}

	void TileMap::render() const
	{
		// rendering tiles
		int fromX, toX;
		int fromY, toY;

		// quick acess variables
		auto& entity_pos = _trackEntity.getComponent<Engine::PositionComponent>();

		fromX = entity_pos.getGridPosition(128).x - nVisibleTiles.x / 2 - 1;
		fromX = Physics::clamp<int>(0, _mapSize.x, fromX);

		fromY = entity_pos.getGridPosition(128).y - nVisibleTiles.y / 2 - 1;
		fromY = Physics::clamp<int>(0, _mapSize.y, fromY);

		toX = entity_pos.getGridPosition(128).x + nVisibleTiles.x / 2 + 2;
		toX = Physics::clamp<int>(0, _mapSize.x, toX);

		toY = entity_pos.getGridPosition(128).y + nVisibleTiles.y / 2 + 2;
		toY = Physics::clamp<int>(0, _mapSize.y, toY);

		this->_tilesRendered = 0;
		for (int x = fromX; x < toX; x++)
		{
			for (int y = fromY; y < toY; y++)
			{
				this->_map[x][y]->render(_data->window);
				this->_tilesRendered++; // updating number of rendered tiles
			}
		}
	}


}
