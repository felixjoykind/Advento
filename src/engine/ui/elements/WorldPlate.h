#pragma once

#include "engine/ui/IHoverable.h"
#include "engine/ui/IClickable.h"
#include "engine/ui/ISelectable.h"

#include "engine/tiles/TileMap.h"

namespace UI
{
    class WorldPlate :
        public UIElement, public IHoverable, public IClickable, public ISelectable
    {
    private:
        sf::Text* _title;

        // data
        Engine::WorldSaveSettings _settings;

    public:
        WorldPlate(GameDataRef data, Engine::WorldSaveSettings settings, 
            sf::Vector2f size, sf::Vector2f pos, sf::Color backgroundColor);
        ~WorldPlate();

        // getters
        Engine::WorldSaveSettings getSettings() const;

        void setPosition(sf::Vector2f position) override;

        void update(float deltaTime) override;
        void render() const override;

    };
}
