#include "SpriteComponent.h"

#include "engine/ecs/Entity.h"
#include "PositionComponent.h"

namespace Engine
{
	SpriteComponent::SpriteComponent(Entity* entity, const sf::Texture& texture)
		:Component(entity), _spr(new sf::Sprite(texture))
	{
		assert(entity->hasComponent<PositionComponent>() && 
			"Entity must contain PositionComponent (SpriteComponent.cpp, line 11)");
	}

	SpriteComponent::SpriteComponent(Entity* entity, const sf::Texture& texture, sf::Vector2f pos)
		:Component(entity), _spr(new sf::Sprite(texture))
	{
		_spr->setPosition(pos);
	}

	SpriteComponent::~SpriteComponent()
	{
		delete this->_spr;
	}

	sf::Sprite& SpriteComponent::getSpr() const
	{
		return *this->_spr;
	}

	void SpriteComponent::update(float deltaTime)
	{
		// updating sprite position with player
		if (_entity->hasComponent<PositionComponent>())
			this->_spr->setPosition(this->_entity->getComponent<PositionComponent>().getPosition());
	}

	void SpriteComponent::render(sf::RenderTarget& target) const
	{
		target.draw(*this->_spr);
	}
}
