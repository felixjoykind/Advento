#include "EmptyItem.h"

namespace Advento
{
	EmptyItem::EmptyItem()
	{
		// init item settings
		this->id = 0;
		this->is_stackable = true;
		this->can_nold = false;
		this->max_num_blocks_in_stack = 64;
		this->curr_num_of_blocks_in_stack = 1;
	}

	EmptyItem::~EmptyItem()
	{ }

	Engine::Item EmptyItem::getCopy(int amount) const
	{
		EmptyItem half_item;

		half_item.id = this->id;
		half_item.is_stackable = this->is_stackable;
		half_item.max_num_blocks_in_stack = 64;
		half_item.curr_num_of_blocks_in_stack = amount;

		return half_item;
	}
}
