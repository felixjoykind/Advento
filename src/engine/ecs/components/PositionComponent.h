#pragma once

#include "engine/ecs/ECS.h"

namespace Engine
{
	class PositionComponent :
		public Component
	{
	private:
		float x, y;

	public:
		PositionComponent(Entity* e, float _x, float _y);
		~PositionComponent();

		// getters
		sf::Vector2f getPosition() const;
		sf::Vector2u getGridPosition(const unsigned tileSize) const;
		float getX() const;
		float getY() const;

		//setters
		void setPosition(float _x, float _y);
		void setX(float _x);
		void setY(float _y);

		void move(float offset_x, float offset_y);
		void move(sf::Vector2f offset);

	};
}
