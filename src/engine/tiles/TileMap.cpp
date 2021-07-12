#include "TileMap.h"

#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "engine/Math.h"
#include "engine/LOG.h"
#include "engine/defenitions/BASIC_WORLD_SETTINGS.h"
#include "engine/defenitions/PATH_DEFENITIONS.h"
#include "engine/ecs/components/PositionComponent.h"

namespace Engine
{
	TileMap::TileMap(GameDataRef data, unsigned int rows, unsigned int cols, const Entity* trackEntity)
		:_data(data), _mapSize({ rows, cols }), _mapSizeInChunks({ rows / CHUNK_SIZE, cols / CHUNK_SIZE }),
		_chunksDistance(1), _trackEntity(trackEntity), _tilesRendered(0)
	{
	}

	TileMap::~TileMap()
	{
		// delete all chunks
		for (auto& chunk : this->_loadedChunks)
			delete chunk;
		this->_loadedChunks.clear();

		for (auto& chunk : this->_changedChunks)
			delete chunk;
		this->_changedChunks.clear();
	}

	unsigned TileMap::tilesRendered() const { return this->_tilesRendered; }
	unsigned TileMap::chunksLoaded() const { return this->_loadedChunks.size(); }
	sf::Vector2u TileMap::getSize() const { return this->_mapSize; }

	void TileMap::setStartingPosition(sf::Vector2u position)
	{
		// validate data
		if (position.x > this->_mapSizeInChunks.x
			|| position.y > this->_mapSizeInChunks.y)
		{
			throw("Position index is out of map bounds (Tilemap.cpp, in setStartingPosition)");
		}

		this->_previousChunk = position;
	}

	void TileMap::generate(GenerationSettings settings)
	{
		// getting char map and clearing previous
		for (auto& chunk : this->_loadedChunks)
			delete chunk;
		this->_loadedChunks.clear();

		for (auto& chunk : this->_changedChunks)
			delete chunk;
		this->_changedChunks.clear();

		//auto map = std::move(MapGenerator::GenerateBlocksMap(settings, 'g', 'w')); // generated map
		auto map = std::move(MapGenerator::GenerateFullMap(settings)); // generated map

		// split map into chunks
		for (unsigned int x = 0; x < settings.width / CHUNK_SIZE; x++)
		{
			for (unsigned int y = 0; y < settings.height / CHUNK_SIZE; y++)
			{
				char** chunkMap = new char* [CHUNK_SIZE * CHUNK_SIZE]; // raw chunk data
				for (unsigned short i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++)
					chunkMap[i] = new char[MAP_LAYER_DEPTH];

				// filling chunk with data from global map
				for (int chunk_x = 0; chunk_x < CHUNK_SIZE; chunk_x++)
				{
					unsigned int chunk_to_map_x = x == 0 ? chunk_x : chunk_x + CHUNK_SIZE * x;
					for (int chunk_y = 0; chunk_y < CHUNK_SIZE; chunk_y++)
					{
						// converting chunk coordinates to position in global map
						unsigned int chunk_to_map_y = y == 0 ? chunk_y : chunk_y + CHUNK_SIZE * y;

						chunkMap[chunk_y * CHUNK_SIZE + chunk_x] = map[chunk_to_map_x][chunk_to_map_y];
					}
				}

				// init new chunk
				this->_changedChunks.push_back(new Chunk(this->_data->assets, { x, y }, chunkMap));
			}
		}

		// clear generated map
		for (size_t i = 0; i < settings.height; i++)
			delete[] map[i];
		delete[] map;
	}

	void TileMap::save_to(const WorldSaveSettings settings)
	{
		if (!std::filesystem::exists(std::filesystem::path(settings.dir_path)))
			std::filesystem::create_directories(settings.dir_path); // creating directory

		this->_worldSaveSettings = { settings.name, settings.dir_path };

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

		// if chunks directory doesn't exist
		if (!std::filesystem::exists(std::filesystem::path(settings.dir_path + "\\chunks")))
			std::filesystem::create_directories(settings.dir_path + "\\chunks"); // creating directory

		// save each chunk
		for (const auto& chunk : this->_changedChunks)
		{
			auto chunk_pos = chunk->getPosition();
			std::string chunk_name = "chunk " + std::to_string(chunk_pos.x) + " " + std::to_string(chunk_pos.y) + ".chunk";

			std::fstream chunk_file;
			chunk_file.open(settings.dir_path + "\\chunks\\" + chunk_name, std::fstream::app | std::fstream::out);
			if (chunk_file.is_open())
			{
				// writing chunk map to file
				for (size_t i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++)
				{
					chunk_file.write(static_cast<char*>(&chunk->_rawTiles[i][0]), sizeof(chunk->_rawTiles[i][0]));
				}
				chunk_file << std::endl;
				for (size_t i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++)
				{
					chunk_file.write(static_cast<char*>(&chunk->_rawTiles[i][1]), sizeof(chunk->_rawTiles[i][1]));
				}
			}

			chunk_file.close();
		}
	}

	void TileMap::load_from(const std::string world_dir_filepath)
	{
		// checking if world directory path exists
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

		// reading world info and saving into _worldSaveSettings
		json.at("name").get_to(this->_worldSaveSettings.name);
		json.at("dir_path").get_to(this->_worldSaveSettings.dir_path);
		
		info_file.close(); // end info_file

		// clear all previous chunks
		for (auto& chunk : this->_loadedChunks)
			delete chunk;
		this->_loadedChunks.clear();

		for (auto& chunk : this->_changedChunks)
			delete chunk;
		this->_changedChunks.clear();

		// init map size in tiles
		this->_mapSize.x = BASIC_WORLD_SIZE_X; // rows
		this->_mapSize.y = BASIC_WORLD_SIZE_Y; // cols

		// init map size in chunks
		this->_mapSizeInChunks.x = BASIC_WORLD_SIZE_X / CHUNK_SIZE; // rows
		this->_mapSizeInChunks.y = BASIC_WORLD_SIZE_Y / CHUNK_SIZE; // cols
	}

	Chunk* TileMap::getChunkFromFile(std::string filepath, sf::Vector2u chunk_pos)
	{
		// load it from file
		std::fstream chunk_file;
		Chunk* new_chunk = nullptr;

		chunk_file.open(filepath, std::fstream::in); // open for reading

		if (chunk_file.is_open())
		{
			char** chunk_map = new char* [CHUNK_SIZE * CHUNK_SIZE];
			for (size_t i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++)
				chunk_map[i] = new char[MAP_LAYER_DEPTH];

			std::string layer;
			unsigned int layer_index = 0;
			while (std::getline(chunk_file, layer))
			{
				for (size_t i = 0; i < layer.length(); i++)
				{
					chunk_map[i][layer_index] = layer[i];
				}
				++layer_index;
			}
			new_chunk = new Chunk(this->_data->assets, chunk_pos, chunk_map);
		}
		chunk_file.close();

		return new_chunk;
	}

	void TileMap::update(float deltaTime)
	{
		// update tile map

		// quick acess variables
		auto& entity_pos = this->_trackEntity->getComponent<Engine::PositionComponent>();

		if (this->_previousChunk != entity_pos.chunkCoordsFromPosition())
		{
			// calculating what chunks needs to be loaded
			this->fromX = entity_pos.chunkCoordsFromPosition().x - this->_chunksDistance;
			this->fromX = Math::clamp<int>(0, _mapSizeInChunks.x - 1, fromX);

			this->fromY = entity_pos.chunkCoordsFromPosition().y - this->_chunksDistance;
			this->fromY = Math::clamp<int>(0, _mapSizeInChunks.y - 1, fromY);

			this->toX = entity_pos.chunkCoordsFromPosition().x + this->_chunksDistance;
			this->toX = Math::clamp<int>(0, _mapSizeInChunks.x - 1, toX);

			this->toY = entity_pos.chunkCoordsFromPosition().y + this->_chunksDistance;
			this->toY = Math::clamp<int>(0, _mapSizeInChunks.y - 1, toY);

			// loop through range
			for (int x = fromX; x <= toX; x++)
			{
				for (int y = fromY; y <= toY; y++)
				{
					// if chunk that should be loaded is not loaded yet
					if (std::find_if(_loadedChunks.begin(), _loadedChunks.end(),
						[&](Chunk* c)
						{
							return c->getPosition() == sf::Vector2u(x, y);
						}
					) == this->_loadedChunks.end())
					{
						LOG("Loaded chunk: (" << x << ", " << y << ")"); // debug info
						// get new chunk from file
						auto new_chunk = this->getChunkFromFile(
							_worldSaveSettings.dir_path + "\\chunks\\chunk " + std::to_string(x) + " " + std::to_string(y) + ".chunk",
							sf::Vector2u((unsigned int)x, (unsigned int)y));
						// load new chunk
						this->_loadedChunks.push_back(new_chunk);
					}
				}
			}

			// lambda to check if chunk is in bounds
			auto isChunkInBounds = [&](const Chunk* c)
			{
				return c->getPosition().x <= toX && c->getPosition().x >= fromX
					&& c->getPosition().y <= toY && c->getPosition().y >= fromY;
			};

			// unload unnecessary chunks
			for (int i = _loadedChunks.size() - 1; i >= 0; --i)
			{
				// if chunk is not in bounds of player vision - it should be deleted
				if (!isChunkInBounds(this->_loadedChunks[i]))
				{
					// delete it
					auto chunk_pos = this->_loadedChunks[i]->getPosition();
					LOG("Unloaded chunk: (" << chunk_pos.x << ", " << chunk_pos.y << ")"); // debug info

					// delete chunk
					delete this->_loadedChunks[i];
					this->_loadedChunks.erase(this->_loadedChunks.begin() + i);
				}
			}

			// update previous chunk
			this->_previousChunk = entity_pos.chunkCoordsFromPosition();
		}

		// update loaded chunks
		this->_tilesRendered = 0; // reset
		for (auto& chunk : this->_loadedChunks)
		{
			this->_tilesRendered += chunk->tilesRendered();
			chunk->update(deltaTime);
		}
	}

	void TileMap::render() const
	{
		// rendering tiles (chunks)
		for (const auto& chunk : this->_loadedChunks)
			chunk->render(this->_data->window);
	}
}
