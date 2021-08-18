#pragma once

#include "engine/ecs/Entity.h"

// TODO: add posibility to destroy itself and drop some loot

namespace Advento
{
    class Tree :
        public Engine::Entity
    {
    public:
        Tree(GameDataRef data, sf::Vector2f pos);
        ~Tree();

        void update(float deltaTime) override;
        void render() const override;

    };
}
