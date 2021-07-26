#pragma once
#include "Item.h"

namespace Advento
{
	class Stick :
		public Engine::Item
	{
	public:
		Stick();
		~Stick();

		Item getCopy(int amount = 1) const override;
	};
}
