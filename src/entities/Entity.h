#pragma once

#include "Game.h"
#include "engine/components/MovementComponent.h"
#include "engine/components/AnimationComponent.h"
#include "engine/components/HitboxComponent.h"

// Base class for all entities
class Entity
{
protected:
	GameDataRef _data;
	sf::Sprite* _spr;

	// Components (nullptr by default, can be overrided)
	Engine::MovementComponent* _movement = nullptr;
	Engine::AnimationComponent* _animation = nullptr;
	Engine::HitboxComponent* _hitbox = nullptr;

public:
	Entity(GameDataRef data, sf::Vector2f pos);
	virtual  ~Entity();

	// loading and creating
	virtual void loadTexture(const sf::Texture& texture);
	virtual void createHitboxComponent(Engine::HitboxSettings settings);
	virtual void createMovementComponent(const Engine::MovementSettings settings);
	virtual void createAnimationComponent(sf::Sprite& sprite, const sf::Texture& texture);

	// getters
	virtual Engine::HitboxComponent* getHitbox() const;
	virtual sf::Vector2f getPosition() const;

	// additions
	virtual void move(const float dir_x, const float dir_y, float deltaTime);

	// base functions
	virtual void update(float deltaTime);
	virtual void render() const;

};
