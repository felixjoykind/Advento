#include "Stick.h"

namespace Advento
{
	Stick::Stick()
	{
		// init item settings
		this->id = 1;
		this->is_stackable = true;
		this->can_nold = false;
		this->max_num_blocks_in_stack = 64;
		this->curr_num_of_blocks_in_stack = 1;
	}

	Stick::~Stick()
	{ }

	Engine::Item Stick::getCopy(int amount) const
	{
		Stick item_copy;

		item_copy.id = this->id;
		item_copy.is_stackable = this->is_stackable;
		item_copy.max_num_blocks_in_stack = this->max_num_blocks_in_stack;
		item_copy.curr_num_of_blocks_in_stack = amount;

		return item_copy;
	}
}
