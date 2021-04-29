#pragma once

#include "engine/ecs/ECS.h"
#include "engine/ecs/Entity.h"

namespace Engine
{
	class Component;

	struct MovementSettings
	{
		float maxVelocity;
		float aceleration;
		float deceleration;
	};

	enum class MovementState
	{
		IDLE = 0, LEFT, RIGHT, UP, DOWN
	};

	class MovementComponent : 
		public Component
	{
	private:
		sf::Vector2f _velocity;
		MovementSettings _settings;

	public:
		MovementComponent(Entity* entity, MovementSettings settings);
		~MovementComponent();

		// Getters
		sf::Vector2f getVelocity() const;
		MovementState getState() const;

		void move(float dir_x, float dir_y);
		void update(float deltaTime) override;

	};
}

