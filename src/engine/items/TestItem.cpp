#include "TestItem.h"

namespace Advento
{
	EmptyItem::EmptyItem()
	{
		// init item settings
		this->id = 0;
		this->is_stackable = true;
		this->max_num_blocks_in_stack = 64;
		this->curr_num_of_blocks_in_stack = 1;
	}

	EmptyItem::~EmptyItem()
	{ }

	const sf::Texture& EmptyItem::getTexture(const AssetManager& assets) const
	{
		return assets.GetTexture("empty item");
	}
}
