#pragma once

#include "engine/items/Item.h"
#include "engine/ecs/Entity.h"

class WorldItem :
    public Engine::Entity
{
public:
    Engine::Item* _item;

public:
    WorldItem(GameDataRef data, Engine::EntitiesManager* entities, sf::Vector2f pos, Engine::Item* item);
    ~WorldItem();

    void update(float deltaTime) override;
    void render() const override;

};

