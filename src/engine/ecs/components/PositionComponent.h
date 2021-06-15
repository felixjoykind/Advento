#pragma once

#include "engine/ecs/ECS.h"

namespace Engine
{
	class PositionComponent :
		public Component
	{
	private:
		float _x, _y;

	public:
		PositionComponent(Entity* e, float x, float y);
		~PositionComponent();

		// Returns current positiobn
		sf::Vector2f getPosition() const;

		// Returns current grid position
		sf::Vector2u getGridPosition() const;

		// Returns coordinates of chunk current position is relied to
		sf::Vector2u chunkCoordsFromPosition() const;

		float getX() const;
		float getY() const;

		//setters
		void setPosition(float x, float y);
		void setX(float x);
		void setY(float y);

		void move(float offset_x, float offset_y);
		void move(sf::Vector2f offset);

	};
}
