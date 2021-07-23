#pragma once

#include <SFML/Graphics.hpp>

class SpriteManipulator
{
public:
	// Stretchs sprite to target size
	static void Stretch(sf::Sprite* sprite, sf::Vector2f target)
	{
		sf::Vector2f target_background_size
		{
			float(target.x),
			float(target.y),
		};

		sprite->setScale(
			target_background_size.x / sprite->getLocalBounds().width,
			target_background_size.y / sprite->getLocalBounds().height
		);
	}

	// Returns item texture name based on its id
	static std::string getItemTextureNameFromId(int id)
	{
		switch (id)
		{
		case 1:
			return "stick item";
		default:
			return "empty item";
		}
	}
};