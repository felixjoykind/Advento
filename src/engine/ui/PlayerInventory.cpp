#include "PlayerInventory.h"

#include "engine/ecs/components/InventoryComponent.h"

namespace UI
{
	PlayerInventory::PlayerInventory(GameDataRef data, Player& player)
		:UIElement(data, { 600.f, 600.f }, { 25.f, 100.f }),
		_playerInvComponent(player.getComponent<Engine::InventoryComponent<PLAYER_INVENTORY_SIZE>>())
	{
		this->_shape->setFillColor(sf::Color(153, 102, 51));
		this->setActive(false);

		refreshItemsSprites();
	}

	PlayerInventory::~PlayerInventory()
	{
	}

	void PlayerInventory::refreshItemsSprites()
	{
		// delete all previous sprites and texts
		for (auto& item : this->_inventoryItems)
		{
			item.sprite_ptr.reset();
			item.text_ptr.reset();
		}
		this->_inventoryItems.clear();

		// refresh
		auto items = this->_playerInvComponent.getAllItems();
		for (int y = 0; y < ROWS; y++)
		{
			for (int x = 0; x < COLS; x++)
			{
				int i = y * COLS + x;

				if (items[i].id != Engine::InventoryComponent<PLAYER_INVENTORY_SIZE>::EMPTY_SLOT_ID)
				{
					// display number of blocks only when its higher than 1
					std::string ui_item_text = items[i].curr_num_of_blocks_in_stack == 1 ? "" : std::to_string(items[i].curr_num_of_blocks_in_stack);

					UI_Item ui_item =
					{
						std::make_unique<sf::Sprite>(items[i].getTexture(this->_data->assets)),
						std::make_unique<sf::Text>(ui_item_text, _data->assets.GetFont("SegoeUI Regular"), 20U)
					};

					ui_item.setPosition(
						{
							this->getPosition().x + X_GAP_BETWEEN_SLOTS + x * SLOT_SIZE,
							this->getPosition().y + 100.f + Y_GAP_BETWEEN_SLOTS + y * SLOT_SIZE
						}
					);

					this->_inventoryItems.emplace_back(std::move(ui_item));
				}
			}
		}
	}

	void PlayerInventory::update(float deltaTime)
	{
		UIElement::update(deltaTime); // returns if not active

		//LOG(this->getPosition().x << ", " << this->getPosition().y);
	}

	void PlayerInventory::render() const
	{
		UIElement::render(); // renders background

		// render slots
		for (const auto& ui_item : this->_inventoryItems)
		{
			ui_item.render(this->_data->window);
		}
	}
}
