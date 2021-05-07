#pragma once

#include "engine/ecs/Entity.h"

#include "engine/ecs/components/PositionComponent.h"
#include "engine/ecs/components/SpriteComponent.h"
#include "engine/ecs/components/MovementComponent.h"
#include "engine/ecs/components/AnimationComponent.h"
#include "engine/ecs/components/HitboxComponent.h"

class Player
	: public Engine::Entity
{
public:
	Player(GameDataRef data, sf::Vector2f pos);
	~Player();

	void update(float deltaTime) override;
	void render() const override;

};
