#include "Entity.h"

Entity::Entity(GameDataRef data, sf::Vector2f pos)
	:_data(data), _spr(new sf::Sprite())
{
	this->_spr->setPosition(pos);
}

Entity::~Entity()
{
	delete this->_spr;
	delete this->_movement;
}

void Entity::loadTexture(const sf::Texture& texture)
{
	this->_spr->setTexture(texture);
}

void Entity::createHitboxComponent(Engine::HitboxSettings settings)
{
	this->_hitbox = new Engine::HitboxComponent(settings);
}

void Entity::createMovementComponent(const Engine::MovementSettings settings)
{
	this->_movement = new Engine::MovementComponent(settings);
}

void Entity::createAnimationComponent(sf::Sprite& spr, const sf::Texture& texture)
{
	this->_animation = new Engine::AnimationComponent(spr, texture);
}

Engine::HitboxComponent* Entity::getHitbox() const
{
	return this->_hitbox;
}

sf::Vector2f Entity::getPosition() const
{
	return this->_spr->getPosition();
}

sf::Vector2u Entity::getGridPosition(const unsigned tileSize) const
{
	return sf::Vector2u(
		static_cast<unsigned>(this->getPosition().x) / gridSize,
		static_cast<unsigned>(this->getPosition().y) / gridSize
	);
}

void Entity::move(const float dir_x, const float dir_y, float deltaTime)
{
	if (this->_movement) // if movement components != nullptr
	{
		this->_movement->move(dir_x, dir_y);
		this->_spr->move(_movement->getVelocity() * deltaTime);
	}
	else
	{
		throw("[!] Movement component is not inisialized");
	}
}

void Entity::update(float deltaTime)
{
	if (this->_movement) // if components != nullptr
	{
		// moving
		this->_movement->update(deltaTime);
		this->_spr->move(_movement->getVelocity() * deltaTime);
	}

	if (this->_hitbox)
	{
		// updating hitbox
		this->_hitbox->update(deltaTime);
	}
}

void Entity::render() const
{
	this->_data->window.draw(*this->_spr);

	if (this->_hitbox) // drawing component if != nullptr
	{
		this->_hitbox->render(_data->window);
	}
}
