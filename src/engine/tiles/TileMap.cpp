#include "TileMap.h"

#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "engine/Physics.h"
#include "engine/defenitions/BASIC_WORLD_SETTINGS.h"
#include "engine/defenitions/PATH_DEFENITIONS.h"
#include "engine/ecs/components/PositionComponent.h"

namespace Engine
{
	TileMap::TileMap(GameDataRef data, unsigned int rows, unsigned int cols, const Entity* trackEntity)
		:_data(data), _mapSize({ rows, cols }), _mapSizeInChunks({ rows / CHUNK_SIZE, cols / CHUNK_SIZE }),
		_trackEntity(trackEntity), _tilesRendered(0), _chunksLoaded(0)
	{
	}

	TileMap::~TileMap()
	{
		// delete all chunks
		for (auto& chunk : this->_chunks)
			delete chunk;
		this->_chunks.clear();

		for (auto& chunk : this->_changedChunks)
			delete chunk;
		this->_changedChunks.clear();
	}

	unsigned TileMap::tilesRendered() const { return this->_tilesRendered; }
	unsigned TileMap::chunksLoaded() const { return this->_chunksLoaded; }
	sf::Vector2u TileMap::getSize() const { return this->_mapSize; }

	void TileMap::generate(GenerationSettings settings)
	{
		// getting char map and clearing previous
		this->_chunks.clear(); // clear chunks
		auto map = std::move(MapGenerator::Generate(settings)); // generated map

		// split map into chunks
		for (unsigned int x = 0; x < settings.width / CHUNK_SIZE; x++)
		{
			for (unsigned int y = 0; y < settings.height / CHUNK_SIZE; y++)
			{
				char* chunkMap = new char[CHUNK_SIZE * CHUNK_SIZE]; // raw chunk data

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

		// if chunks directory doeen't exist
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
					chunk_file.write(static_cast<char*>(&chunk->_rawTiles[i]), sizeof(chunk->_rawTiles[i]));
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
		this->_chunks.clear();
		this->_chunks.shrink_to_fit();
		this->_changedChunks.clear();
		this->_changedChunks.shrink_to_fit();

		// init map size in tiles
		this->_mapSize.x = BASIC_WORLD_SIZE_X; // rows
		this->_mapSize.y = BASIC_WORLD_SIZE_Y; // cols

		// init map size in chunks
		this->_mapSizeInChunks.x = BASIC_WORLD_SIZE_X / CHUNK_SIZE; // rows
		this->_mapSizeInChunks.y = BASIC_WORLD_SIZE_Y / CHUNK_SIZE; // cols
	}

	void TileMap::update(float deltaTime)
	{ 
		// update tile map

		// quick acess variables
		auto& entity_pos = this->_trackEntity->getComponent<Engine::PositionComponent>();

		// calculating what chunks needs to be updated and rendereds
		fromX = entity_pos.chunkCoordsFromPosition().x;
		fromX = Physics::clamp<int>(0, _mapSizeInChunks.x, fromX);

		fromY = entity_pos.chunkCoordsFromPosition().y;
		fromY = Physics::clamp<int>(0, _mapSizeInChunks.y, fromY);

		toX = entity_pos.chunkCoordsFromPosition().x;
		toX = Physics::clamp<int>(0, _mapSizeInChunks.x, toX);

		toY = entity_pos.chunkCoordsFromPosition().y;
		toY = Physics::clamp<int>(0, _mapSizeInChunks.y, toY);

		// load chunk if _trackEntity goes in unloaded location
		auto it = std::find_if(_chunks.begin(), _chunks.end(),
			[&](Chunk* c)
			{
				return c->getPosition() == entity_pos.chunkCoordsFromPosition();
			}
		);
		if (it == this->_chunks.end()) // if chunk is not found
		{
			// load it from file
			std::fstream chunk_file;
			auto chunk_pos = entity_pos.chunkCoordsFromPosition();

			chunk_file.open(
				this->_worldSaveSettings.dir_path + "\\chunks\\chunk " + std::to_string(chunk_pos.x) + " " + std::to_string(chunk_pos.y) + ".chunk",
				std::fstream::in); // open for reading

			if (chunk_file.is_open())
			{
				char* chunk_map = new char[CHUNK_SIZE * CHUNK_SIZE];
				for (size_t i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++)
				{
					chunk_file >> chunk_map[i];
				}
				Chunk* new_chunk = new Chunk(this->_data->assets, chunk_pos, chunk_map);
				this->_chunks.push_back(new_chunk); // add loaded chunk to list
			}

			chunk_file.close();
		}

		// unload unnecessary chunks
		auto delete_it = std::remove_if(_chunks.begin(), _chunks.end(),
			[&](Chunk* c)
			{
				return c->getPosition() != entity_pos.chunkCoordsFromPosition();
			}
		);
		if (delete_it != this->_chunks.end())
		{
			this->_chunks.erase(delete_it);
		}

		// updating debug data (chunks loaded, tiles rendered)
		this->_chunksLoaded = this->_chunks.size();
		this->_tilesRendered = this->_chunksLoaded * (CHUNK_SIZE * CHUNK_SIZE);

		// update loaded chunks
		for (auto& chunk : this->_chunks)
			chunk->update(deltaTime);
	}

	void TileMap::render() const
	{
		// rendering tiles

		for (const auto& chunk : this->_chunks)
			chunk->render(this->_data->window);
	}
}
