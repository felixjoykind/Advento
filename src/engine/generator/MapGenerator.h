#pragma once

#include <string>

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
		static char** Generate(GenerationSettings s);
	};
}

