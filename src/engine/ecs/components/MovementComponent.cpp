#include "MovementComponent.h"

#include "PositionComponent.h"
#include "SpriteComponent.h"
#include "engine/Math.h"

namespace Engine
{
	MovementComponent::MovementComponent(Entity* entity, MovementSettings settings)
		:Component(entity), _settings(settings)
	{ }

	MovementComponent::~MovementComponent()
	{ }

	sf::Vector2f MovementComponent::getVelocity() const
	{
		return this->_velocity;
	}

	MovementState MovementComponent::getState() const
	{
		if (_velocity.x < 0.f)
			return MovementState::LEFT;
		else if (_velocity.x > 0.f)
			return MovementState::RIGHT;
		else if (_velocity.y < 0.f)
			return MovementState::UP;
		else if (_velocity.y > 0.f)
			return MovementState::DOWN;
		else
			return MovementState::IDLE;
	}

	void MovementComponent::move(float dir_x, float dir_y)
	{
		// accelerating
		this->_velocity.x += _settings.aceleration * dir_x;
		this->_velocity.x = Math::clamp<float>(-_settings.maxVelocity, _settings.maxVelocity, _velocity.x);

		this->_velocity.y += _settings.aceleration * dir_y;
		this->_velocity.y = Math::clamp<float>(-_settings.maxVelocity, _settings.maxVelocity, _velocity.y);
	}

	void MovementComponent::doDecelerationLogic(float& value)
	{
		if (value > 0.0f)
		{
			value -= this->_settings.deceleration;
			if (value < 0.0f) value = 0.0f;
		}
		else if (value < 0.0f)
		{
			value += this->_settings.deceleration;
			if (value > 0.0f) value = 0.0f;
		}
	}

	void MovementComponent::update(float deltaTime)
	{
		// decceleration and clamping
		doDecelerationLogic(this->_velocity.x); // x
		doDecelerationLogic(this->_velocity.y); // y

		// moving entity
		this->_entity->getComponent<PositionComponent>().move(this->getVelocity() * deltaTime);
	}
}
