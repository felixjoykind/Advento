#include "PositionComponent.h"

#include "engine/defenitions/BASIC_WORLD_SETTINGS.h"
#include "engine/ecs/Entity.h"
#include "SpriteComponent.h"

namespace Engine
{
	PositionComponent::PositionComponent(Entity* e, float _x, float _y)
		:Component(e), x(_x), y(_y)
	{
	}

	PositionComponent::~PositionComponent()
	{
	}

	sf::Vector2f PositionComponent::getPosition() const
	{
		return sf::Vector2f(this->x, this->y);
	}

	sf::Vector2u PositionComponent::getGridPosition() const
	{
		return sf::Vector2u(
			static_cast<unsigned>(this->getX()) / TILE_SIZE,
			static_cast<unsigned>(this->getY()) / TILE_SIZE
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

	float PositionComponent::getX() const { return this->x; }
	float PositionComponent::getY() const { return this->y; }
	void PositionComponent::setX(float _x) { this->x = _x; }
	void PositionComponent::setY(float _y) { this->y = _y; }

	void PositionComponent::setPosition(float _x, float _y)
	{
		this->x = _x;
		this->y = _y;
	}

	void PositionComponent::move(float offset_x, float offset_y)
	{
		this->x += offset_x;
		this->y += offset_y;
	}

	void PositionComponent::move(sf::Vector2f offset)
	{
		this->x += offset.x;
		this->y += offset.y;

		// if entity has sprite component
		if (this->_entity->hasComponent<SpriteComponent>())
		{
			// update sprite position
			// also its a bug fix with sprite moving and rendering
			this->_entity->getComponent<SpriteComponent>().getSpr().setPosition({ this->x, this->y });
		}
	}
}
