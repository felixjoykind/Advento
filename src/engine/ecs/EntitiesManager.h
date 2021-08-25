#pragma once

#include "Game.h"

namespace Engine
{
	class Entity;

	using EntitiesVector = std::vector<std::unique_ptr<Entity>>;

	class EntitiesManager
	{
	private:
		GameDataRef _data;
		EntitiesVector _entities;

	public:
		EntitiesManager(GameDataRef data) :_data(data) { }
		~EntitiesManager() { }

		// Returns all entities
		EntitiesVector& getEntities();

		// setters
		template<class T, class... TArgs>
		T& createEntity(TArgs&&... args)
		{
			static_assert(std::is_base_of<Entity, T>::value,
				"Type T must be inherited from Entity class.");

			T* e = new T(_data, this, std::forward<TArgs>(args)...);
			std::unique_ptr<Entity> uPtr{ e };
			_entities.emplace_back(std::move(uPtr));
			return *e;
		}

		void refresh();
		void update(float deltaTime);
		void render() const;

	};
}

