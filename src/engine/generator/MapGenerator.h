#pragma once

#include <string>

#define MAP_LAYER_DEPTH 2

#define GRASS_TILE_CHAR 'g'
#define WATER_TILE_CHAR 'w'
#define TREE_TILE_CHAR 't'
#define EMPTY_TILE_CHAR '.'

namespace Engine
{
	struct GenerationSettings
	{
		std::string seed;
		unsigned int width;
		unsigned int height;
		float initChance;
		int birthLimit;
		int deathLimit; 
		int smooth_level;
	};

	class MapGenerator
	{
	private:
		MapGenerator() { }

	public:
		static char** GenerateBlocksMap(GenerationSettings s, char filled, char empty);

		// Generates tree map
		static char** GenerateTreeMap(GenerationSettings s);
		static char*** GenerateFullMap(GenerationSettings s);

		static std::string getRandomSeed(unsigned int count);
	};
}

