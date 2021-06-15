#include "PositionComponent.h"

#include "engine/defenitions/BASIC_WORLD_SETTINGS.h"
#include "engine/ecs/Entity.h"
#include "SpriteComponent.h"

namespace Engine
{
	PositionComponent::PositionComponent(Entity* e, float x, float y)
		:Component(e), _x(x), _y(y)
	{
	}

	PositionComponent::~PositionComponent()
	{
	}

	sf::Vector2f PositionComponent::getPosition() const
	{
		return sf::Vector2f(this->_x, this->_y);
	}

	sf::Vector2u PositionComponent::getGridPosition() const
	{
		return sf::Vector2u(
			static_cast<unsigned>(this->getX() / TILE_SIZE),
			static_cast<unsigned>(this->getY() / TILE_SIZE)
		);
	}

	sf::Vector2u PositionComponent::chunkCoordsFromPosition() const
	{
		return sf::Vector2u
		{
			unsigned int(this->getGridPosition().x) / CHUNK_SIZE,
			unsigned int(this->getGridPosition().y) / CHUNK_SIZE
		};
	}

	float PositionComponent::getX() const { return this->_x; }
	float PositionComponent::getY() const { return this->_y; }
	void PositionComponent::setX(float x) { this->_x = x; }
	void PositionComponent::setY(float y) { this->_y = y; }

	void PositionComponent::setPosition(float x, float y)
	{
		this->_x = x;
		this->_y = y;
	}

	void PositionComponent::move(float offset_x, float offset_y)
	{
		this->_x += offset_x;
		this->_y += offset_y;
	}

	void PositionComponent::move(sf::Vector2f offset)
	{
		this->_x += offset.x;
		this->_y += offset.y;

		// if entity has sprite component
		if (this->_entity->hasComponent<SpriteComponent>())
		{
			// update sprite position
			// also its a bug fix with sprite moving and rendering
			this->_entity->getComponent<SpriteComponent>().getSpr().setPosition({ this->_x, this->_y });
		}
	}
}
