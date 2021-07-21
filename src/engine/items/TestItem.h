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

		const sf::Texture& getTexture(const AssetManager& assets) const override;
	};
}
