#include "Item.h"

namespace Engine
{
	Item::Item()
	{ }

	Item::~Item()
	{ }

	const sf::Texture& Item::getTexture(const AssetManager& assets) const
	{
		return assets.GetTexture("empty item");
	}
}
