#include "WorldItemsManager.h"

#include "engine/LOG.h"

WorldItemsManager::WorldItemsManager(GameDataRef data)
	:_data(data)
{ }

WorldItemsManager::~WorldItemsManager()
{ }

void WorldItemsManager::addWorldItem(Engine::Item* dropped_item, sf::Vector2f drop_pos)
{
	sf::Sprite* wrld_item_spr = new sf::Sprite(dropped_item->getTexture(_data->assets));
	wrld_item_spr->setPosition(drop_pos);

	World_Item* wrld_item = new World_Item{dropped_item, wrld_item_spr};

	std::unique_ptr<World_Item> itemPtr{ wrld_item };
	this->_worldItems.emplace_back(std::move(itemPtr));
}

void WorldItemsManager::update(float deltaTime)
{
	// TODO: implement
	// update
	// maybe collision detection here

	//system("cls");
	LOG(this->_worldItems.size());
}

void WorldItemsManager::render() const
{
	for (const auto& world_item : this->_worldItems)
	{
		this->_data->window.draw(*world_item->item_sprite);
	}
}
