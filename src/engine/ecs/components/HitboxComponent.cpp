#include "HitboxComponent.h"

namespace Engine
{
	HitboxComponent::HitboxComponent(Entity* entity, sf::Vector2f offset, sf::Vector2f size, bool visible)
		:Component(entity), _visible(visible)
	{
		// setting up hitbox
		this->_hitbox.setPosition(entity->getPosition() + offset);
		this->_hitbox.setSize(size);
		this->_hitbox.setFillColor(sf::Color(0, 0, 0, 0));
		this->_hitbox.setOutlineColor(sf::Color::Green);
		this->_hitbox.setOutlineThickness(1.f);
	}

	HitboxComponent::HitboxComponent(Entity* entity, HitboxSettings settings)
		:Component(entity), _visible(settings.visible)
	{
		// setting up hitbox
		this->_hitbox.setPosition(entity->getPosition() + settings.offset);
		this->_hitbox.setSize(settings.size);
		this->_hitbox.setFillColor(sf::Color(0, 0, 0, 0));
		this->_hitbox.setOutlineColor(sf::Color::Green);
		this->_hitbox.setOutlineThickness(1.f);
	}

	HitboxComponent::~HitboxComponent()
	{ }

	bool HitboxComponent::getVisible() const
	{
		return this->_visible;
	}

	bool HitboxComponent::collides(const HitboxComponent& other)
	{
		// collision detection
		return this->_hitbox.getGlobalBounds().intersects(other._hitbox.getGlobalBounds());
	}

	void HitboxComponent::setVisible(bool value)
	{
		this->_visible = value;
	}

	void HitboxComponent::update(float deltaTime)
	{
		this->_hitbox.setPosition(this->_entity->getPosition());
	}

	void HitboxComponent::render(sf::RenderTarget& target) const
	{
		if (this->_visible)
		{
			target.draw(this->_hitbox);
		}
	}
}
