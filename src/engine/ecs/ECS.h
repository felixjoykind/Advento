#pragma once

#include <bitset>
#include <array>

#include <SFML/Graphics.hpp>

namespace Engine
{
	class Entity; // include entity class (solution for double include)

	// Base class for all components
	class Component
	{
	protected:
		Entity* _entity; // entity, component is relied to

	public:
		Component(Entity* e) : _entity(e) {  }
		virtual ~Component() {  }

		// basic functions
		virtual void update(float deltaTime) {  }
		virtual void render(sf::RenderTarget& target) const {  }

		// giving entity access to private functions and variables
		friend class Entity;
	};

	constexpr std::size_t maxComponents = 32;

	using ComponentID = std::size_t;
	using ComponentBetset = std::bitset<maxComponents>;
	using ComponentArray = std::array<Component*, maxComponents>;

	inline ComponentID getComponentTypeID()
	{
		static ComponentID lastID = 0;
		return lastID++;
	}

	template<class T>
	inline ComponentID getComponentTypeID() noexcept
	{
		static_assert(std::is_base_of<Component, T>::value,
			"Type T must be inherited from Component class.");

		static ComponentID typeID = getComponentTypeID();
		return typeID;
	}

}
