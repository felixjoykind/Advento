#include "Item.h"

#include "engine/SpriteManipulator.h"

namespace Engine
{
	Item::Item()
	{ }

	Item::~Item()
	{ }

	int Item::getHalfOfAmount() const
	{
		return (int)std::ceil((float)this->curr_num_of_blocks_in_stack / 2.f);
	}

	std::string Item::getItemTextureNameFromId(int id)
	{
		switch (id)
		{
		case 1:
			return "stick item";
		default:
			return "empty item";
		}
	}

	const sf::Texture& Item::getTexture(const AssetManager& assets) const
	{
		return assets.GetTexture(Item::getItemTextureNameFromId(this->id));
	}

	Item Item::getCopy(int amount) const
	{
		Item half_item;
		half_item.id = this->id;
		half_item.is_stackable = this->is_stackable;
		half_item.max_num_blocks_in_stack = this->max_num_blocks_in_stack;
		half_item.curr_num_of_blocks_in_stack = amount;

		return half_item;
	}
}
