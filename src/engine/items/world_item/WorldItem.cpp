#include "WorldItem.h"

#include "engine/ecs/components/PositionComponent.h"
#include "engine/ecs/components/SpriteComponent.h"
#include "engine/ecs/components/HitboxComponent.h"

#include "engine/LOG.h"

WorldItem::WorldItem(GameDataRef data, Engine::EntitiesManager* entities, sf::Vector2f pos, Engine::Item* item)
	:Engine::Entity(data, entities, pos), _item(new Engine::Item(item->curr_num_of_blocks_in_stack))
{
	// copy item data
	this->_item->id = item->id;
	this->_item->is_stackable = item->is_stackable;
	this->_item->can_nold = item->can_nold;
	this->_item->max_num_blocks_in_stack = item->max_num_blocks_in_stack;

	using namespace Engine;

	this->addComponent<PositionComponent,
		Engine::Entity*, float, float>(this, 0.f, 0.f);
	this->getComponent<PositionComponent>().setPosition(pos.x, pos.y);

	this->addComponent<SpriteComponent,
		Entity*, const sf::Texture&>(this, this->_item->getTexture(data->assets));

	this->addComponent<HitboxComponent,
		Entity*, sf::Vector2f, sf::Vector2f>(this, { 0.f, 0.f }, { 64.f, 64.f });
}

WorldItem::~WorldItem()
{
	delete this->_item;
}

void WorldItem::update(float deltaTime)
{
	Engine::Entity::update(deltaTime);
}

void WorldItem::render() const
{
	Engine::Entity::render();
}
