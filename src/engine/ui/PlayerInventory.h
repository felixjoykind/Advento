#pragma once

#include "elements/UIElement.h"
#include "entities/Player.h"

namespace UI
{
	class PlayerInventory :
		public UIElement
	{
	private:
		// background
		sf::Sprite* _background;

		// pointer to a player inventory component
		Engine::InventoryComponent<PLAYER_INVENTORY_SIZE>& _playerInvComponent;

		using Sprite_Ptr = std::unique_ptr<sf::Sprite>;
		using Text_Ptr = std::unique_ptr<sf::Text>;

		// Represents rendering struct for items
		struct UI_Item
		{
			Sprite_Ptr sprite_ptr;
			Text_Ptr text_ptr;

			void setPosition(sf::Vector2f pos)
			{
				this->sprite_ptr->setPosition(pos);
				this->text_ptr->setPosition(
					{
						pos.x + SLOT_SIZE / 2.f,
						pos.y + SLOT_SIZE / 2.f
					}
				);
			}

			void render(sf::RenderTarget& target) const
			{
				target.draw(*sprite_ptr);
				target.draw(*text_ptr);
			}
		};

		std::vector<UI_Item> _inventoryItems;

	public:
		PlayerInventory(GameDataRef data, Player& player);
		~PlayerInventory();

		// Refreshes list of items sprites (shoud be called after every change in inventory)
		void refreshItemsSprites();

		void update(float deltaTime) override;
		void render() const override;

	};
}
