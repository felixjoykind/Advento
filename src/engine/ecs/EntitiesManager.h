#pragma once

#include "Game.h"
#include "Entity.h"

namespace Engine
{
	using EntityVector = std::vector<std::unique_ptr<Entity>>;

	class EntityManager
	{
	private:
		GameDataRef _data;
		EntityVector _entities;

	public:
		EntityManager(GameDataRef data) :_data(data) { }
		~EntityManager() { }

		// getters
		const EntityVector& getEntities() const;

		// setters
		template<class T, class... TArgs>
		Entity& createEntity(TArgs&&... args)
		{
			static_assert(std::is_base_of<Entity, T>::value,
				"Type T must be inherited from Entity class.");

			T* e = new T(_data, std::forward<TArgs>(args)...);
			std::unique_ptr<Entity> uPtr{ e };
			_entities.emplace_back(std::move(uPtr));
			return *e;
		}

		void refresh();
		void update(float deltaTime);
		void render() const;

	};
}

