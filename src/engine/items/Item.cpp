#include "Item.h"

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

	const sf::Texture& Item::getTexture(const AssetManager& assets) const
	{
		return assets.GetTexture("empty item");
	}

	Item Item::getHalf()
	{
		Item half_item;
		half_item.id = this->id;
		half_item.is_stackable = this->is_stackable;
		half_item.max_num_blocks_in_stack = 64;
		half_item.curr_num_of_blocks_in_stack = this->getHalfOfAmount();

		return half_item;
	}
}
