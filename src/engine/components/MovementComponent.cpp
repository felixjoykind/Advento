#include "MovementComponent.h"
#include "engine/Physics.h"

namespace Engine
{
	MovementComponent::MovementComponent(MovementSettings settings)
		:_settings(settings)
	{ }

	MovementComponent::~MovementComponent()
	{ }

	sf::Vector2f MovementComponent::getVelocity() const
	{
		return this->_velocity;
	}

	MovementState MovementComponent::getState() const
	{
		if (_velocity.x == 0.f && _velocity.y == 0.f)
			return MovementState::IDLE;
		else if (_velocity.x < 0.f)
			return MovementState::LEFT;
		else if (_velocity.x > 0.f)
			return MovementState::RIGHT;
		else if (_velocity.y < 0.f)
			return MovementState::UP;
		else if (_velocity.y > 0.f)
			return MovementState::DOWN;
	}

	void MovementComponent::move(float dir_x, float dir_y)
	{
		this->_velocity.x += _settings.aceleration * dir_x;
		this->_velocity.x = Physics::clamp<float>(-_settings.maxVelocity, _settings.maxVelocity, _velocity.x);

		this->_velocity.y += _settings.aceleration * dir_y;
		this->_velocity.y = Physics::clamp<float>(-_settings.maxVelocity, _settings.maxVelocity, _velocity.y);
	}

	void MovementComponent::update(float deltaTime)
	{
		// decceleration and clamping
		if (_velocity.x > 0.0f)
		{
			_velocity.x -= _settings.deceleration;
			if (_velocity.x < 0.0f) _velocity.x = 0.0f;
		}
		else if (_velocity.x < 0.0f)
		{
			_velocity.x += _settings.deceleration;
			if (_velocity.x > 0.0f) _velocity.x = 0.0f;
		}

		if (_velocity.y > 0.0f)
		{
			_velocity.y -= _settings.deceleration;
			if (_velocity.y < 0.0f) _velocity.y = 0.0f;
		}
		else if (_velocity.y < 0.0f)
		{
			_velocity.y += _settings.deceleration;
			if (_velocity.y > 0.0f) _velocity.y = 0.0f;
		}
	}

}
