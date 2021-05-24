#pragma once

#include <SFML/Graphics.hpp>

#include "IClickable.h"
#include "engine/tiles/TileMap.h"

namespace UI
{
    class WorldPlate :
        public UIElement, public IClickable
    {
    private:
        sf::Text* _title;

        // data
        Engine::WorldSaveSettings _settings;
        bool _selected = false;

    public:
        WorldPlate(GameDataRef data, Engine::WorldSaveSettings settings, 
            sf::Vector2f size, sf::Vector2f pos, sf::Color backgroundColor);
        ~WorldPlate();

        // getters
        Engine::WorldSaveSettings getSettings() const;

        void update(float deltaTime) override;
        void render() const override;

    };
}
