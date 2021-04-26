#pragma once

#include <SFML/Graphics.hpp>

namespace Engine
{
	struct HitboxSettings
	{
		sf::Sprite& spr;
		sf::Vector2f offset;
		sf::Vector2f size;
		bool visible;
	};

	class HitboxComponent
	{
	private:
		const sf::Sprite& _spr;
		sf::RectangleShape _hitbox;

		// visual
		bool _visible;

	public:
		HitboxComponent(HitboxSettings settings);
		HitboxComponent(sf::Sprite& spr, sf::Vector2f offset, sf::Vector2f size, bool visible);
		~HitboxComponent();

		// setters
		void setVisible(bool value);

		// coliision detection function
		bool collides(const HitboxComponent& other);

		// basic functionality
		void update(float deltaTime);
		void render(sf::RenderTarget& target) const;
	};
}

