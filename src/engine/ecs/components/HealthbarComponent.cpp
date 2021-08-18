#include "HealthbarComponent.h"

#include "engine/Math.h"

#include "engine/ecs/components/PositionComponent.h"

namespace Engine
{
	HealthbarComponent::HealthbarComponent(Entity* entity, float max, float current)
		:Component(entity), _max(max), _current(current), 
		_backgroud(new sf::RectangleShape({ 100.f, 30.f })), _health(new sf::RectangleShape({ _current, 30.f }))
	{
		if (this->_entity->hasComponent<PositionComponent>())
		{
			auto entity_pos = this->_entity->getComponent<PositionComponent>().getPosition();
			this->_backgroud->setPosition(
				{
					entity_pos.x - 50.f,
					entity_pos.y - 50.f
				}
			);

			this->_health->setPosition(
				{
					entity_pos.x - 50.f,
					entity_pos.y - 50.f
				}
			);
		}
	}

	HealthbarComponent::~HealthbarComponent()
	{ }

	float HealthbarComponent::getHealth() const { return this->_current; }

	void HealthbarComponent::takeDamage(const float damage)
	{
		this->_current -= damage;
		this->_current = Math::clamp<float>(this->_min, this->_max, this->_current);

		// update healthbar
		this->_health->setSize(
			{
				this->_current,
				this->_health->getSize().y
			}
		);
	}

	void HealthbarComponent::heal(const float heal_amount)
	{
		this->_current += heal_amount;
		this->_current = Math::clamp<float>(this->_min, this->_max, this->_current);

		// update healthbar
		this->_health->setSize(
			{
				this->_current,
				this->_health->getSize().y
			}
		);
	}

	void HealthbarComponent::render(sf::RenderTarget& target) const
	{
		if (this->_current < this->_max)
		{
			target.draw(*this->_backgroud);
			target.draw(*this->_health);
		}
	}
}
