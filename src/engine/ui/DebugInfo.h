#pragma once

#include "engine/ecs/ECS.h"
#include "engine/tiles/TileMap.h"

namespace Engine
{
	struct DebugData
	{
		Entity& _trackEntity;
		TileMap& _map;
	};

	// Debug info panel (like in Minecraft)
	class DebugInfo
	{
	private:
		// data
		GameDataRef _data;
		DebugData _debugData;
		bool _active;

		// ui
		std::map<std::string, sf::Text*> _texts;

	public:
		DebugInfo(GameDataRef data, DebugData debugData);
		~DebugInfo();

		// getters
		bool isActive() const;

		// setters
		void setActive(bool value);

		// basic functions
		void update(float deltaTime);
		void render(sf::RenderTarget& target) const;

	};
}
