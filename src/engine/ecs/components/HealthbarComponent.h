#pragma once

#include "engine/ecs/Entity.h"

namespace Engine
{
	class Component;

	class HealthbarComponent :
		public Component
	{
	private:
		float _min = 0.f, _max, _current; // health values

		// render data
		sf::RectangleShape* _backgroud;
		sf::RectangleShape* _health;

	public:
		HealthbarComponent(Entity* entity, float max, float current);
		~HealthbarComponent();

		// Returns current health
		float getHealth() const;

		void takeDamage(const float damage);
		void heal(const float heal_amount);

		void render(sf::RenderTarget& target) const override;

	};
}
