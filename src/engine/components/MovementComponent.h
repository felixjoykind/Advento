#pragma once

#include <SFML/Graphics.hpp>

namespace Engine
{
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

	class MovementComponent
	{
	private:
		sf::Vector2f _velocity;
		MovementSettings _settings;

	public:
		MovementComponent(MovementSettings settings);
		~MovementComponent();

		// Getters
		sf::Vector2f getVelocity() const;
		MovementState getState() const;

		void move(float dir_x, float dir_y);
		void update(float deltaTime);

	};
}

