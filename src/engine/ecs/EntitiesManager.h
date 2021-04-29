#pragma once

#include "Entity.h"

namespace Engine
{
	class EntityManager
	{
	private:
		std::vector<std::unique_ptr<Entity>> _entities;

	public:
		EntityManager() { }
		~EntityManager() { }

		void update(float deltaTime);
		void render(sf::RenderTarget& target) const;

		// setters
		Entity& createEntity(Entity* new_entity);

	};
}

