#include "EntitiesManager.h"

#include "Entity.h"

namespace Engine
{
	EntitiesVector& EntitiesManager::getEntities()
	{
		return this->_entities;
	}

	void EntitiesManager::refresh()
	{
		// deleting unactive entities
		_entities.erase(std::remove_if(_entities.begin(), _entities.end(),
			[](const std::unique_ptr<Entity>& e)
			{
				return !e->isAlive();
			}), _entities.end());
	}

	void EntitiesManager::update(float deltaTime)
	{
		this->refresh(); // refreshing entities

		// updating entitites
		for (auto& e : _entities)
			e->update(deltaTime);
	}

	void EntitiesManager::render() const
	{
		// rendering entitites
		for (const auto& e : _entities)
			e->render();
	}
}
