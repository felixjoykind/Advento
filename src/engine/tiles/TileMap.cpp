#include "TileMap.h"

#include <fstream>
#include <filesystem>

#include "engine/Physics.h"
#include "engine/defenitions/BASIC_WORLD_SETTINGS.h"
#include "engine/ecs/components/PositionComponent.h"

namespace Engine
{
	TileMap::TileMap(GameDataRef data, unsigned int rows, unsigned int cols)
		:_data(data), _mapSize({ rows, cols }),
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
		this->_map.clear();
		this->_rawMap = std::move(MapGenerator::Generate(settings));;
		
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
				switch (this->_rawMap[x][y])
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

	void TileMap::save_to(const WorldSaveSettings settings) const
	{
		if (!std::filesystem::exists(std::filesystem::path(settings.dir_path)))
			std::filesystem::create_directories(settings.dir_path); // creating directories

		// opening wrld_info file (.json)
		std::fstream wrld_info;

		wrld_info.open(settings.dir_path + "\\world_info.json", std::fstream::app | std::fstream::out);
		if (wrld_info.is_open())
		{
			nlohmann::json json{};
			json["name"] = settings.name;
			json["dir_path"] = settings.dir_path;

			// writing to json info file
			wrld_info << json.dump();
		}
		wrld_info.close();

		// opening world file
		std::fstream file;
		file.open(settings.dir_path + "\\" + settings.name + ".wrld", std::fstream::app | std::fstream::out);
		if (file.is_open())
		{
			for (int i = 0; i < this->_mapSize.x; ++i)
				file.write(static_cast<char*>(this->_rawMap[i]), sizeof(char) * this->_mapSize.y);
		}
		file.close();
	}

	void TileMap::load_from(const std::string world_dir_filepath)
	{
		// checking if world directory path existss
		if (!std::filesystem::exists(std::filesystem::path(world_dir_filepath)))
			throw("Couldn't load world. World directory doens't exist. (TileMap.cpp, TileMap::load_from)");

		nlohmann::json json{}; // json converter

		// reading world data (.json)
		std::fstream info_file;
		info_file.open(world_dir_filepath + "\\world_info.json", std::fstream::in); // openning info_file
		if (info_file.is_open())
		{
			// loading file data to json
			info_file >> json;
		}
		// reading world name
		std::string world_name;
		json.at("name").get_to(world_name);
		
		info_file.close(); // end info_file

		// init raw map
		this->_rawMap = new char* [256];
		for (int i = 0; i < 256; i++)
			this->_rawMap[i] = new char[256];

		std::fstream world_file;
		world_file.open(world_dir_filepath + "\\" + world_name + ".wrld", std::fstream::in); // opening world tiles file
		if (world_file.is_open())
		{
			// reading tilemap line by line
			for (int i = 0; i < this->_mapSize.x; ++i)
			{
				world_file.read(static_cast<char*>(this->_rawMap[i]), sizeof(char) * this->_mapSize.y);
			}
		}
		world_file.close(); // end world_file

		this->_mapSize.x = BASIC_WORLD_SIZE_X; // rows
		this->_mapSize.y = BASIC_WORLD_SIZE_Y; // cols

		// reinit tiles vector
		this->_map.clear();
		this->_map = std::vector<std::vector<Tile*>>(this->_mapSize.x);
		for (auto& row : _map)
			row = std::vector<Tile*>(this->_mapSize.y);

		// filling map based on input
		for (size_t x = 0; x < this->_mapSize.x; x++)
		{
			for (size_t y = 0; y < this->_mapSize.y; y++)
			{
				std::string texture_name; // texture name (grass tile by default)

				// choosing texture name based on input
				switch (this->_rawMap[x][y])
				{
				case 'g':
					texture_name = "grass tile";
					break;
				case '.':
					texture_name = "water tile";
					break;
				}

				// adding new tile
				this->_map[x][y] = new Tile(
					_data->assets.GetTexture(texture_name),
					{ (float)x * 128.f, (float)y * 128.f }
				);
			}
		}
	}

	unsigned TileMap::tilesRendered() const
	{
		return this->_tilesRendered;
	}

	void TileMap::update(float deltaTime)
	{ 
		// update tile map
	}

	void TileMap::render(const Entity& trackEntity) const
	{
		// rendering tiles
		int fromX, toX;
		int fromY, toY;

		// quick acess variables
		auto& entity_pos = trackEntity.getComponent<Engine::PositionComponent>();

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
