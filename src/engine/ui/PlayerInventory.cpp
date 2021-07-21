#include "PlayerInventory.h"

#include "engine/ecs/components/InventoryComponent.h"

namespace UI
{
	PlayerInventory::PlayerInventory(GameDataRef data, Player& player)
		:UIElement(data, { 607.f, 600.f }, { 25.f, 25.f }),
		_background(new sf::Sprite(data->assets.GetTexture("inventory"))),
		_playerInvComponent(player.getComponent<Engine::InventoryComponent<PLAYER_INVENTORY_SIZE>>())
	{
		this->_background->setPosition(this->getPosition());
		this->setActive(false);

		refreshItemsSprites();
	}

	PlayerInventory::~PlayerInventory()
	{
		delete this->_background;
	}

	void PlayerInventory::refreshItemsSprites()
	{
		// delete all previous sprites and texts
		for (auto& item : this->_inventoryItems)
		{
			item.sprite_ptr.reset();
			item.amount_text_ptr.reset();
		}
		this->_inventoryItems.clear();

		if (this->_movingItem != nullptr)
		{
			this->_movingItem = nullptr;
		}

		// refresh
		auto& items = this->_playerInvComponent.getAllItems();
		for (int y = 0; y < ROWS; y++)
		{
			for (int x = 0; x < COLS; x++)
			{
				int i = y * COLS + x;

				if (items[i].id != Engine::InventoryComponent<PLAYER_INVENTORY_SIZE>::EMPTY_SLOT_ID)
				{
					// display number of blocks only when its higher than 1
					std::string item_amount_text = items[i].curr_num_of_blocks_in_stack == 1 ? "" : std::to_string(items[i].curr_num_of_blocks_in_stack);
					sf::Vector2f item_pos =
					{
						this->getPosition().x + INVENTORY_OFFSET_X + x * (SLOT_SIZE),
						this->getPosition().y + INVENTORY_OFFSET_Y + Y_GAP_BETWEEN_SLOTS - 5.f + y * (SLOT_SIZE)
					};

					UI_Item ui_item =
					{
						items[i],
						{ x, y },
						false,
						std::make_unique<sf::Sprite>(items[i].getTexture(this->_data->assets)),
						std::make_unique<sf::Text>(item_amount_text, _data->assets.GetFont("SegoeUI Regular"), 20U)
					};
					ui_item.setPosition(item_pos);

					this->_inventoryItems.emplace_back(std::move(ui_item));
				}
			}
		}
	}

	sf::Vector2i PlayerInventory::mouseToSlot(sf::Vector2i mouse_pos) const
	{
		for (int y = 0; y < ROWS; y++)
		{
			for (int x = 0; x < COLS; x++)
			{
				sf::IntRect rect(
					{
						int(this->getPosition().x + INVENTORY_OFFSET_X + x * (SLOT_SIZE)),
						int(this->getPosition().y + INVENTORY_OFFSET_Y + Y_GAP_BETWEEN_SLOTS - 5.f + y * (SLOT_SIZE))
					},
					{ (int)ITEM_SIZE, (int)ITEM_SIZE }
				);

				if (rect.contains(mouse_pos))
					return { x, y };
			}
		}
		
		return { POS_INVALID_VALUE, POS_INVALID_VALUE };
	}

	PlayerInventory::UI_Item* PlayerInventory::getHoveredItem(sf::Vector2i mouse_pos)
	{
		for (auto& ui_item : this->_inventoryItems)
		{
			if (ui_item.sprite_ptr->getGlobalBounds().contains(float(mouse_pos.x), float(mouse_pos.y)))
			{ // item is hovered
				return &ui_item;
			}
		}

		return nullptr;
	}

	void PlayerInventory::handleInput(sf::Event ev)
	{
		if (ev.type == sf::Event::MouseButtonPressed)
		{
			auto mouse_pos = InputManager::getMousePosition(this->_data->window);
			if (ev.mouseButton.button == sf::Mouse::Button::Left)
			{
				if (this->_movingItem == nullptr) // if we are not moving anything
				{
					this->_movingItem = getHoveredItem(mouse_pos);
					if (this->_movingItem != nullptr)
						this->_movingItem->following_mouse = true;
				}
				else
				{ // we are moving some ui_item and we want to do smth with it
					sf::Vector2i slot_pos = this->mouseToSlot(mouse_pos); // position of slot we want to move item
					if (slot_pos.x == POS_INVALID_VALUE || slot_pos.y == POS_INVALID_VALUE)
					{ // invalid click
						return;
					}

					// check if clicked on extisting slot
					if (slot_pos.x < COLS && slot_pos.y < ROWS && slot_pos.x >= 0 && slot_pos.y >= 0)
					{
						if (_playerInvComponent.getItemAt(slot_pos).id == 
							_playerInvComponent.getItemAt(_movingItem->cords).id &&
							slot_pos != this->_movingItem->cords)
						{
							if (this->_playerInvComponent.addItemTo(
								slot_pos, this->_movingItem->item.curr_num_of_blocks_in_stack))
							{ // added successfully
								// added item doesnt exist anymore
								this->_playerInvComponent.removeItem(this->_movingItem->cords);
							}
							else
							{ // failed to add
								this->_playerInvComponent.swapItems(
									this->_movingItem->cords,
									{ slot_pos.x, slot_pos.y }
								); // swap
							}
						}
						else
						{
							this->_playerInvComponent.swapItems(
								this->_movingItem->cords, 
								{ slot_pos.x, slot_pos.y }
							); // swap
						}
						this->refreshItemsSprites(); // refresh
					}
				}
			}
			else if (ev.mouseButton.button == sf::Mouse::Button::Right)
			{ // we want to split item
				if (this->_movingItem == nullptr)
				{
					this->_playerInvComponent.splitItem(getHoveredItem(mouse_pos)->cords);
					this->refreshItemsSprites();
				}
				else
				{
					// TODO: add 1 to clicked slot
				}
			}
		}
	}

	void PlayerInventory::update(float deltaTime)
	{
		UIElement::update(deltaTime); // returns if not active

		for (auto& ui_item : this->_inventoryItems)
		{
			ui_item.update(deltaTime, this->_data->window);
		}
	}

	void PlayerInventory::render() const
	{
		this->_data->window.draw(*this->_background); // renders background

		// render items
		for (const auto& ui_item : this->_inventoryItems)
		{
			ui_item.render(this->_data->window);
		}
	}

	void PlayerInventory::open()
	{
		this->setActive(true);
		this->refreshItemsSprites();
	}

	void PlayerInventory::close()
	{
		this->setActive(false);
		this->_movingItem = nullptr;
	}
}
