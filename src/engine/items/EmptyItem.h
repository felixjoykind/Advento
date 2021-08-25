#pragma once
#include "Item.h"

namespace Advento
{
	class EmptyItem :
		public Engine::Item
	{
	public:
		EmptyItem(int amount = 1);
		~EmptyItem();

		Item getCopy(int amount = 1) const override;
	};
}
