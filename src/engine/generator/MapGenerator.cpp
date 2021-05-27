#include "MapGenerator.h"

#include "engine/tiles/Tile.h"
#include "engine/Random.h"

namespace Engine
{
	char** MapGenerator::Generate(GenerationSettings s)
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
						if (map[nX][nY] == 'g')
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
				map[x][y] = Random::Float() < s.initChance ? '.' : 'g';
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

					if (map[x][y] == 'g')
					{
						if (nbs < s.deathLimit)
							map[x][y] = '.';
					}
					else if (map[x][y] == '.')
					{
						if (nbs > s.birthLimit)
							map[x][y] = 'g';
					}
				}
			}
		}

		return map;
	}
}
