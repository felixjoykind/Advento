#pragma once

#include "entities/Entity.h"

namespace Engine
{
	class Component;

	struct HitboxSettings
	{
		sf::Vector2f offset;
		sf::Vector2f size;
		bool visible;
	};

	class HitboxComponent : 
		public Component
	{
	private:
		sf::RectangleShape _hitbox;

		// visual
		bool _visible;

	public:
		HitboxComponent(Entity* entity, HitboxSettings settings);
		HitboxComponent(Entity* entity, sf::Vector2f offset, sf::Vector2f size, bool visible);
		~HitboxComponent();

		// setters
		void setVisible(bool value);

		// coliision detection function
		bool collides(const HitboxComponent& other);

		// basic functionality
		void update(float deltaTime) override;
		void render(sf::RenderTarget& target) const override;
	};
}

