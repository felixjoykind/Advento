#pragma once
#include "Item.h"

namespace Advento
{
	class WoodenAxe :
		public Engine::Item
	{
	public:
		WoodenAxe(int amount = 1);
		~WoodenAxe();

		Item getCopy(int amount = 1) const override;
	};
}
