#include "TestItem.h"

namespace Advento
{
	TestItem::TestItem()
	{
		// init item settings
		this->id = 0;
		this->is_stackable = true;
		this->max_num_blocks_in_stack = 64;
		this->curr_num_of_blocks_in_stack = 1;
	}

	TestItem::~TestItem()
	{ }

	const sf::Texture& TestItem::getTexture(const AssetManager& assets) const
	{
		return assets.GetTexture("test item");
	}
}
