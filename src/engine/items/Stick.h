#pragma once
#include "Item.h"

namespace Advento
{
	class Stick :
		public Engine::Item
	{
	public:
		Stick(int amount = 1);
		~Stick();

		Item getCopy(int amount = 1) const override;
	};
}
