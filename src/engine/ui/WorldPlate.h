#pragma once

#include "UIElement.h"
#include <SFML/Graphics.hpp>

namespace UI
{
    class WorldPlate :
        public UIElement
    {
    private:
        sf::Text* _title;

        bool _selected = false;

    public:
        WorldPlate(GameDataRef data, std::string title, 
            sf::Vector2f size, sf::Vector2f pos, sf::Color backgroundColor);
        ~WorldPlate();

        void update(float deltaTime) override;
        void render() const override;

    };
}
