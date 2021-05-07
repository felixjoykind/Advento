#pragma once

#include "Game.h"
#include "engine/ecs/ECS.h"
#include <SFML/Graphics.hpp>
#include <cassert>

namespace Engine
{
	// Base class for all entities
	class Entity
	{
	protected:
		// data
		GameDataRef _data;
		bool _alive = true;

		// components data
		std::vector<std::unique_ptr<Component>> _components;
		ComponentArray _componentArray { };
		ComponentBetset _componentBitset;

	public:
		Entity(GameDataRef data, sf::Vector2f pos = { 0.f, 0.f });
		virtual  ~Entity();

		// getters
		virtual bool isAlive() const;

		// ECS
		template<class T>
		bool hasComponent() const
		{
			return this->_componentBitset[getComponentTypeID<T>()];
		}

		template<class T>
		T& getComponent() const
		{
			assert(this->hasComponent<T>());
			auto ptr(_componentArray[getComponentTypeID<T>()]);
			return *static_cast<T*>(ptr);
		}

		template<class T, class... TArgs>
		T& addComponent(TArgs&&... mArgs)
		{
			assert(!this->hasComponent<T>());

			T* c = new T(std::forward<TArgs>(mArgs)...);
			c->_entity = this;
			std::unique_ptr<Component> uPtr{ c };
			_components.emplace_back(std::move(uPtr));

			// now component is added and active
			_componentArray[getComponentTypeID<T>()] = c;
			_componentBitset[getComponentTypeID<T>()] = true;

			return *c;
		}

		// base functions
		virtual void update(float deltaTime);
		virtual void render() const;

		// destroy
		virtual void destroy();

	};
}
