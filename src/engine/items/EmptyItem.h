#pragma once
#include "Item.h"

namespace Advento
{
	class EmptyItem :
		public Engine::Item
	{
	public:
		EmptyItem();
		~EmptyItem();

		Item getCopy(int amount = 1) const override;
	};
}
