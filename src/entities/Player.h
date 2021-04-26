#pragma once

#include "entities/Entity.h"

class Player
	: public Entity
{
public:
	Player(GameDataRef data, sf::Vector2f pos);
	~Player();

	void update(float deltaTime) override;
	void render() const override;

};
