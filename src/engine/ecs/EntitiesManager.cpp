#include "EntitiesManager.h"

namespace Engine
{
	void EntityManager::update(float deltaTime)
	{
		// deleting unactive entities
		_entities.erase(std::remove_if(_entities.begin(), _entities.end(),
			[](const std::unique_ptr<Entity>& e)
			{
				return !e->isActive();
			}), _entities.end());

		// updating entitites
		for (auto& e : _entities)
			e->update(deltaTime);
	}

	void EntityManager::render(sf::RenderTarget& target) const
	{
		// rendering entitites
		for (const auto& e : _entities)
			e->render();
	}

	Entity& EntityManager::createEntity(Entity* new_entity)
	{
		Entity* e = std::move(new_entity);
		std::unique_ptr<Entity> uPtr{ e };
		_entities.emplace_back(std::move(uPtr));
		return *e;
	}
}
