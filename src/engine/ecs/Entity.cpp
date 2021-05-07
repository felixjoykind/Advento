#include "Entity.h"

namespace Engine
{
	Entity::Entity(GameDataRef data, sf::Vector2f pos)
		:_data(data)
	{
	}

	Entity::~Entity()
	{
	}

	bool Entity::isAlive() const
	{
		return this->_alive;
	}

	void Entity::destroy()
	{
		this->_alive = false;
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
		//_data->window.draw(*this->_spr);

		// rendering components
		for (const auto& c : _components)
			c->render(_data->window);
	}

}
