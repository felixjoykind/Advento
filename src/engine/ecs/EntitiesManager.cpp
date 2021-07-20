#include "EntitiesManager.h"

namespace Engine
{
	const EntityVector& EntityManager::getEntities() const
	{
		return this->_entities;
	}

	void EntityManager::refresh()
	{
		// deleting unactive entities
		_entities.erase(std::remove_if(_entities.begin(), _entities.end(),
			[](const std::unique_ptr<Entity>& e)
			{
				return !e->isAlive();
			}), _entities.end());
	}

	void EntityManager::update(float deltaTime)
	{
		this->refresh(); // refreshing entities

		// TODO: update and render only entities near the player

		// updating entitites
		for (auto& e : _entities)
			e->update(deltaTime);
	}

	void EntityManager::render() const
	{
		// rendering entitites
		for (const auto& e : _entities)
			e->render();
	}
}
