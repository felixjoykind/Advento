#pragma once

#include "TileMap.h"

namespace Engine
{
	class Tile
	{
	private:
		sf::Sprite* _spr;

	public:
		Tile(const sf::Texture& texture, sf::Vector2f pos);
		~Tile();

		// basic functions
		void update(float deltaTime);
		void render(sf::RenderTarget& target);

		friend class TileMap;

	};
}

