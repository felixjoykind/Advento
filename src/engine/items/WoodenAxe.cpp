#include "WoodenAxe.h"

namespace Advento
{
	WoodenAxe::WoodenAxe(int amount)
		:Item(amount)
	{
		this->id = (int)ItemID::WoodenAxe;
		this->is_stackable = false;
		this->can_nold = true;
		this->max_num_blocks_in_stack = 1;
	}

	WoodenAxe::~WoodenAxe()
	{ }

	Engine::Item WoodenAxe::getCopy(int amount) const
	{
		/*WoodenAxe item_copy;
		item_copy.id = this->id;
		item_copy.is_stackable = this->is_stackable;
		item_copy.max_num_blocks_in_stack = this->max_num_blocks_in_stack;
		item_copy.curr_num_of_blocks_in_stack = amount;*/

		return WoodenAxe(amount);
	}
}
