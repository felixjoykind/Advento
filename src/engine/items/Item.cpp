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

	std::string Item::getItemTextureNameFromId(WeaponID id)
	{
		switch (id)
		{
		case WeaponID::Stick:
			return "stick item";
		case WeaponID::Axe:
			return "axe item";
		default:
			return "empty item";
		}
	}

	const sf::Texture& Item::getTexture(const AssetManager& assets) const
	{
		return assets.GetTexture(Item::getItemTextureNameFromId((WeaponID)this->id));
	}

	Item Item::getCopy(int amount) const
	{
		Item item_copy;
		item_copy.id = this->id;
		item_copy.is_stackable = this->is_stackable;
		item_copy.max_num_blocks_in_stack = this->max_num_blocks_in_stack;
		item_copy.curr_num_of_blocks_in_stack = amount;

		return item_copy;
	}
}
