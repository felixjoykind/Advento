#include "HitboxComponent.h"

namespace Engine
{
	HitboxComponent::HitboxComponent(sf::Sprite& spr, sf::Vector2f offset, sf::Vector2f size, bool visible)
		:_spr(spr), _visible(visible)
	{
		// setting up hitbox
		this->_hitbox.setPosition(spr.getPosition() + offset);
		this->_hitbox.setSize(size);
		this->_hitbox.setFillColor(sf::Color(0, 0, 0, 0));
		this->_hitbox.setOutlineColor(sf::Color::Green);
		this->_hitbox.setOutlineThickness(1.f);
	}

	HitboxComponent::HitboxComponent(HitboxSettings settings)
		:_spr(settings.spr), _visible(settings.visible)
	{
		// setting up hitbox
		this->_hitbox.setPosition(settings.spr.getPosition() + settings.offset);
		this->_hitbox.setSize(settings.size);
		this->_hitbox.setFillColor(sf::Color(0, 0, 0, 0));
		this->_hitbox.setOutlineColor(sf::Color::Green);
		this->_hitbox.setOutlineThickness(1.f);
	}

	HitboxComponent::~HitboxComponent()
	{ }

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
		this->_hitbox.setPosition(this->_spr.getPosition());
	}

	void HitboxComponent::render(sf::RenderTarget& target) const
	{
		if (this->_visible)
		{
			target.draw(this->_hitbox);
		}
	}
}
