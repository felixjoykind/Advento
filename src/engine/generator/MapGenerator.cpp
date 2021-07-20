#include "MapGenerator.h"

#include <cassert>

#include "engine/tiles/Tile.h"
#include "engine/Random.h"
#include "engine/defenitions/BASIC_WORLD_SETTINGS.h"

namespace Engine
{
	char** MapGenerator::GenerateBlocksMap(GenerationSettings s, char filled, char empty)
	{
		srand(std::hash<std::string>{}(s.seed));

		// creating dynamic 2d array (map)
		char** map = new char* [s.width];
		for (unsigned short i = 0; i < s.width; i++)
			map[i] = new char[s.height];

		// counting neighbours lambda
		auto count_neighbours = [&](int x, int y)
		{
			int count = 0;
			for (int nX = x - 1; nX <= x + 1; nX++)
			{
				for (int nY = y - 1; nY <= y + 1; nY++)
				{
					if (nX == x && nY == y) continue;
					else if (nX >= 0 && nX < s.width && nY >= 0 && nY < s.height)
					{
						if (map[nX][nY] == filled)
							count++;
					}
					else
						count++;
				}
			}
			return count;
		};

		// randomly filling
		for (int x = 0; x < s.width; x++)
		{
			for (int y = 0; y < s.height; y++)
			{
				map[x][y] = Random::Float() < s.initChance ? empty : filled;
			}
		}

		// smoothing
		for (size_t i = 0; i < s.smooth_level; i++)
		{
			// loop through map
			for (int x = 0; x < s.width; x++)
			{
				for (int y = 0; y < s.height; y++)
				{
					auto nbs = count_neighbours(x, y);

					if (map[x][y] == filled)
					{
						if (nbs < s.deathLimit)
							map[x][y] = empty;
					}
					else if (map[x][y] == empty)
					{
						if (nbs > s.birthLimit)
							map[x][y] = filled;
					}
				}
			}
		}

		return map; // return generated map
	}

	char** MapGenerator::GenerateTreeMap(GenerationSettings s)
	{
		return std::move(GenerateBlocksMap(s, TREE_TILE_CHAR, EMPTY_TILE_CHAR));
	}

	// Generates full map (with trees and other game objects)
	char*** MapGenerator::GenerateFullMap(GenerationSettings s)
	{
		// dynamicly allocate 3d array
		char*** full_map = new char** [s.width]; // full map (to merge with block and tree maps)
		for (int i = 0; i < s.width; i++)
		{
			full_map[i] = new char* [s.height];

			for (int j = 0; j < s.height; j++) {

				// Allocate memory blocks for
				// columns of each 2D array
				full_map[i][j] = new char[MAP_LAYER_DEPTH];
			}
		}

		auto blocks_map = std::move(GenerateBlocksMap(s, GRASS_TILE_CHAR, WATER_TILE_CHAR)); // blocks map
		auto trees_map = std::move(GenerateTreeMap({ getRandomSeed(16), BASIC_WORLD_SIZE_X, BASIC_WORLD_SIZE_Y, 0.55f, 4, 4, 3 })); // trees map

		// merging blocks and trees
		for (int i = 0; i < s.width; i++)
		{
			for (int j = 0; j < s.height; j++)
			{
				full_map[i][j][0] = blocks_map[i][j]; // block (layer 0)
				full_map[i][j][1] = full_map[i][j][0] == WATER_TILE_CHAR ? EMPTY_TILE_CHAR : trees_map[i][j]; // tree (layer 1)
			}
		}

		return full_map;
	}

	std::string MapGenerator::getRandomSeed(unsigned int count)
	{
		std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"); // all possible characters in seed
		assert(count < str.length() &&
			"Too many characters (GenerationState.cpp, getRandomSeed(unsgined int count) const)");

		std::random_device rd;
		std::mt19937 generator(rd());

		std::shuffle(str.begin(), str.end(), generator);
		return str.substr(0, count);
	}
}
