#pragma once

#include "engine/ecs/ECS.h"
#include <SFML/Graphics.hpp>

#include "Game.h"

namespace Engine
{
	// Base class for all entities
	class Entity
	{
	protected:
		// data
		GameDataRef _data;
		sf::Sprite* _spr;
		bool _active = true;

		// components data
		std::vector<std::unique_ptr<Component>> _components;
		ComponentArray _componentArray {  };
		ComponentBetset _componentBitset;

	public:
		Entity(GameDataRef data, sf::Vector2f pos);
		virtual  ~Entity();

		// loading and creating
		virtual void loadTexture(const sf::Texture& texture);

		// getters
		virtual bool isActive() const;
		virtual sf::Sprite& getSpr() const;
		virtual sf::Vector2f getPosition() const;
		virtual sf::Vector2u getGridPosition(const unsigned tileSize) const;

		// ECS
		template<class T>
		bool hasComponent() const
		{
			return this->_componentBitset[getComponentTypeID<T>()];
		}

		template<class T>
		T& getComponent() const
		{
			auto ptr(_componentArray[getComponentTypeID<T>()]);
			return *static_cast<T*>(ptr);
		}

		template<class T, class... TArgs>
		T& addComponent(TArgs&&... mArgs)
		{
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
