#pragma once

#include "engine/ecs/ECS.h"

namespace Engine
{
	class SpriteComponent :
		public Component
	{
	private:
		sf::Sprite* _spr;

	public:
		SpriteComponent(Entity* entity, const sf::Texture& texture);
		SpriteComponent(Entity* entity, const sf::Texture& texture, sf::Vector2f pos);
		~SpriteComponent();

		// getters
		sf::Sprite& getSpr() const;

		void update(float deltaTime);
		void render(sf::RenderTarget& target) const override;

	};
}

