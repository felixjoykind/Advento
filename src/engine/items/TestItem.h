#pragma once
#include "Item.h"

namespace Advento
{
	class TestItem :
		public Engine::Item
	{
	public:
		TestItem();
		~TestItem();

		const sf::Texture& getTexture(const AssetManager& assets) const override;
	};
}
