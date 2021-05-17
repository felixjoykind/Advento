#pragma once

#include "UIElement.h"
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
		: public UI::UIElement
	{
	private:
		// data
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
		void update(float deltaTime) override;
		void render() const override;

	};
}
