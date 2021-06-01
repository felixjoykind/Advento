#pragma once

#include "Chunk.h"
#include "engine/ecs/Entity.h"
#include "engine/generator/MapGenerator.h"

namespace Engine
{
	struct WorldSaveSettings
	{
		std::string name;
		std::string dir_path;
	};

	class TileMap
	{
	private:
		// data
		GameDataRef _data;
		sf::Vector2u _mapSize;
		sf::Vector2u _mapSizeInChunks;
		unsigned _chunksDistance; // chunks render and update distance
		sf::Vector2u _previousChunk;
		WorldSaveSettings _worldSaveSettings;
		const Entity* _trackEntity;
		
		// chunks
		std::vector<Chunk*> _loadedChunks;
		std::vector<Chunk*> _changedChunks; // this vector contains all chunks that have been changed and needs to be saved

		int fromX = 0, toX = 0;
		int fromY = 0, toY = 0;

		// tiles rendering data
		unsigned _tilesRendered;

		Chunk* getChunkFromFile(std::string filepath, sf::Vector2u chunk_pos);

	public:
		TileMap(GameDataRef data, unsigned int rows, unsigned int cols, const Entity* trackEntity);
		~TileMap();

		// Returns the number of rendered tiles
		unsigned tilesRendered() const;
		unsigned chunksLoaded() const;

		// Returns the size of the map
		sf::Vector2u getSize() const;

		// Sets starting position of track entity
		void setStartigPosition(sf::Vector2u position);

		// Generates new map based on settings
		void generate(GenerationSettings settings);

		// json formaters
		void save_to(const WorldSaveSettings settings);
		void load_from(const std::string filepath);

		// basic functions
		void update(float deltaTime);
		void render() const;

	};
}
