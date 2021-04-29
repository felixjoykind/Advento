#include "Entity.h"

namespace Engine
{
	Entity::Entity(GameDataRef data, sf::Vector2f pos)
		:_data(data), _spr(new sf::Sprite())
	{
		this->_spr->setPosition(pos);
	}

	Entity::~Entity()
	{
		delete this->_spr;
	}

	void Entity::loadTexture(const sf::Texture& texture)
	{
		this->_spr->setTexture(texture);
	}

	sf::Sprite& Entity::getSpr() const
	{
		return *this->_spr;
	}

	sf::Vector2f Entity::getPosition() const
	{
		return this->_spr->getPosition();
	}

	sf::Vector2u Entity::getGridPosition(const unsigned tileSize) const
	{
		return sf::Vector2u(
			static_cast<unsigned>(this->getPosition().x) / tileSize,
			static_cast<unsigned>(this->getPosition().y) / tileSize
		);
	}

	bool Entity::isActive() const
	{
		return this->_active;
	}

	void Entity::destroy()
	{
		this->_active = false;
	}

	void Entity::update(float deltaTime)
	{
		// updating components
		for (auto& c : _components)
			c->update(deltaTime);
	}

	void Entity::render() const
	{
		// drawing sprite
		this->_data->window.draw(*this->_spr);

		// new components system
		for (const auto& c : _components)
			c->render(_data->window);
	}

}
