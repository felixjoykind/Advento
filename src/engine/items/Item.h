#pragma once
#include "gamedata/AssetManager.h"

constexpr float ITEM_SIZE = 64.f;

enum class ItemID 
{ 
	Empty = -1,
	Stick = 1,
	WoodenAxe
};

namespace Engine
{
	// Base class for all items
	class Item
	{
	protected:
		// Returns item texture name based on its id
		static std::string getItemTextureNameFromId(ItemID id);

	public:
		int id = -1;
		bool is_stackable = true;
		bool can_nold = false;
		int max_num_blocks_in_stack = 64;
		int curr_num_of_blocks_in_stack = 1;

		Item(int amount = 1);
		~Item();

		int getHalfOfAmount() const;

		virtual const sf::Texture& getTexture(const AssetManager& assets) const;
		virtual Item getCopy(int amount = 1) const;
	};
}