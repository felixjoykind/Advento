#include "Stick.h"

namespace Advento
{
	Stick::Stick()
	{
		// init item settings
		this->id = 1;
		this->is_stackable = true;
		this->max_num_blocks_in_stack = 64;
		this->curr_num_of_blocks_in_stack = 1;
	}

	Stick::~Stick()
	{ }

	Engine::Item Stick::getHalf()
	{
		Stick half_item;

		half_item.id = this->id;
		half_item.is_stackable = this->is_stackable;
		half_item.max_num_blocks_in_stack = this->max_num_blocks_in_stack;
		half_item.curr_num_of_blocks_in_stack = this->getHalfOfAmount();

		return half_item;
	}
}
