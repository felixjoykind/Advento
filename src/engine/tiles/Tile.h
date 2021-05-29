#pragma once

#include <SFML/Graphics.hpp>

namespace Engine
{
	class Tile
	{
	private:
		sf::Sprite* _spr;

	public:
		Tile(const sf::Texture& texture, sf::Vector2f pos);
		~Tile();

		// getters
		sf::Vector2u getGridPosition() const;

		// basic functions
		void update(float deltaTime);
		void render(sf::RenderTarget& target);

	};
}

