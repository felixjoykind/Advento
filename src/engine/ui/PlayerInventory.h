#pragma once

#include "elements/UIElement.h"
#include "entities/Player.h"
#include "gamedata/InputManager.h"

namespace UI
{
	using Sprite_Ptr = std::unique_ptr<sf::Sprite>;
	using Text_Ptr = std::unique_ptr<sf::Text>;

	class PlayerInventory :
		public UIElement
	{
	private:
		// Represents struct of items for rendering
		struct UI_Item
		{
			// invalid to cause error if uninitialized
			const Engine::Item& item;
			sf::Vector2i cords = { POS_INVALID_VALUE, POS_INVALID_VALUE };

			bool following_mouse = false;

			Sprite_Ptr sprite_ptr = nullptr;
			Text_Ptr amount_text_ptr = nullptr;

			void setPosition(sf::Vector2f pos)
			{
				this->sprite_ptr->setPosition(pos);
				this->amount_text_ptr->setPosition(
					{
						pos.x + SLOT_SIZE / 2.f,
						pos.y + SLOT_SIZE / 2.f
					}
				);
			}

			void update(float deltaTime, const sf::Window& window)
			{
				if (InputManager::isSpriteHovered(this->sprite_ptr.get(), window)
					&& this->following_mouse == false)
				{
					this->sprite_ptr->setColor(sf::Color(105, 105, 105));
				}
				else
				{
					this->sprite_ptr->setColor(sf::Color::White);
				}
				if (this->following_mouse == true)
				{ // if following mouse
					auto mouse_pos = InputManager::getMousePosition(window);
					this->setPosition(
						{
							float(mouse_pos.x) - ITEM_SIZE / 2.f,
							float(mouse_pos.y) - ITEM_SIZE / 2.f
						}
					);
				}
			}

			void render(sf::RenderTarget& target) const
			{
				target.draw(*sprite_ptr);
				target.draw(*amount_text_ptr);
			}
		};

		// Converts mouse position to slot position in inventory
		sf::Vector2i mouseToSlot(sf::Vector2i mouse_pos) const;

		UI_Item* getHoveredItem(sf::Vector2i mouse_pos);

	private:
		// background
		sf::Sprite* _background;

		// reference to a player inventory component
		Engine::InventoryComponent<PLAYER_INVENTORY_SIZE>& _playerInvComponent;

		UI_Item* _movingItem = nullptr; // item moved in inventory by user
		std::vector<UI_Item> _inventoryItems;

	public:
		PlayerInventory(GameDataRef data, Player& player);
		~PlayerInventory();

		// Refreshes list of items sprites (shoud be called after every change in inventory)
		void refreshItemsSprites();

		void handleInput(sf::Event ev);
		void update(float deltaTime) override;
		void render() const override;

		void open();
		void close();
	};
}
