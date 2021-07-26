#pragma once
#include "gamedata/AssetManager.h"

constexpr float ITEM_SIZE = 64.f;

namespace Engine
{
	// Base class for all items
	class Item
	{
	protected:
		// Returns item texture name based on its id
		static std::string getItemTextureNameFromId(int id);

	public:
		int id = -1;
		bool is_stackable = true;
		int max_num_blocks_in_stack = 64;
		int curr_num_of_blocks_in_stack = 1;

		Item();
		~Item();

		int getHalfOfAmount() const;

		virtual const sf::Texture& getTexture(const AssetManager& assets) const;
		virtual Item getCopy(int amount = 1) const;
	};
}