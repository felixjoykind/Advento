#pragma once

#include "Game.h"
#include "Item.h"

struct World_Item
{
	Engine::Item* item;
	sf::Sprite* item_sprite;
};

class WorldItemsManager
{
private:
	GameDataRef _data;
	std::vector<std::unique_ptr<World_Item>> _worldItems;

public:
	WorldItemsManager(GameDataRef data);
	~WorldItemsManager();

	void addWorldItem(Engine::Item* dropped_item, sf::Vector2f drop_pos);

	void update(float deltaTime);
	void render() const;

};

