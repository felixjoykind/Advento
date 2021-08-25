#include "PlayerInventory.h"

#include "engine/ecs/components/InventoryComponent.h"

static const sf::Vector2i INVALID_POS = sf::Vector2i{ POS_INVALID_VALUE, POS_INVALID_VALUE };

namespace UI
{
	PlayerInventory::PlayerInventory(GameDataRef data, Player& player, Engine::EntitiesManager& entitiesManager)
		:UIElement(data, { 607.f, 600.f }, { 25.f, 25.f }),
		_background(new sf::Sprite(data->assets.GetTexture("inventory"))),
		_hud(new sf::Sprite(data->assets.GetTexture("inventory hud"))),
		_playerInvComponent(player.getComponent<Engine::InventoryComponent<PLAYER_INVENTORY_SIZE>>()),
		_entitiesManager(entitiesManager)
	{
		this->_background->setPosition(this->getPosition());
		this->_hud->setPosition({ this->getPosition().x + INVENTORY_OFFSET_X - 11.f, this->getPosition().y });
		this->_hudSelector = new HudSelector(this->_hud->getPosition());

		this->refresh();

		this->close();
	}

	PlayerInventory::~PlayerInventory()
	{
		delete this->_background;
		delete this->_hud;
		delete this->_hudSelector;
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

		if (this->isActive() == false) // if closed
		{
			this->refreshHud();
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
		
		return INVALID_POS;
	}

	PlayerInventory::UI_Item* PlayerInventory::getHoveredItem(sf::Vector2i mouse_pos)
	{
		for (auto& ui_item : this->_inventoryItems)
		{
			if (ui_item.cords == this->mouseToSlot(mouse_pos))
			{ // item is hovered
				return &ui_item;
			}
		}

		return nullptr;
	}

	void PlayerInventory::refreshHud()
	{
		// update position of first 7 ui items (hud)
		for (int i = 0; i < 7; i++)
		{
			if (i >= (int)this->_inventoryItems.size())
				break;

			auto& ui_item = this->_inventoryItems[i];
			// find item with needed position
			for (int j = 0; j < 7; j++)
			{
				if (ui_item.cords.y == 0 && ui_item.cords.x == j)
				{
					ui_item.resetColor();
					ui_item.setPosition({ ui_item.getPosition().x, INVENTORY_OFFSET_X + 4.f });
				}
			}
		}
	}

	void PlayerInventory::refreshHandledItem()
	{
		for (const auto& ui_item : this->_inventoryItems)
		{
			if (ui_item.cords.x == this->_hudSelector->selected_index &&
				ui_item.cords.y == 0)
			{ // selected item exists
				if (ui_item.item.can_nold)
				{ // player can handle item
					this->_playerInvComponent.setHoldedItem(&ui_item.item, this->_data->assets);
					return;
				}
			}
		}
		this->_playerInvComponent.removeHoldedItem();
	}

	void PlayerInventory::handleInput(sf::Event ev)
	{
		if (this->isActive() == false)
		{
			// handle hud events (scroll and nums press)
			if (ev.type == sf::Event::MouseWheelMoved)
			{
				// move hud inventory selection index
				this->_hudSelector->increaseSelectedIndex(ev.mouseWheel.delta);
				this->refreshHandledItem();
			}
			return;
		}
		
		// if active (inventory opened)
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
				{ // we are moving some ui_item and we want to do something with it
					sf::Vector2i slot_pos = this->mouseToSlot(mouse_pos); // position of slot we want to move item
					if (slot_pos == INVALID_POS)
					{ // invalid click
						// if clicked out of inventory
						this->_playerInvComponent.dropItem(this->_movingItem->cords, this->_entitiesManager); // drop item
						
						this->refresh();

						return;
					}

					// check if clicked on extisting slot
					if (slot_pos.x < COLS && slot_pos.y < ROWS && slot_pos.x >= 0 && slot_pos.y >= 0)
					{
						if (_playerInvComponent.getItemAt(slot_pos).id == 
							_playerInvComponent.getItemAt(_movingItem->cords).id &&
							slot_pos != this->_movingItem->cords)
						{
							if (this->_playerInvComponent.addToItem(
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
						this->refresh(); // refresh
					}
				}
			}
			else if (ev.mouseButton.button == sf::Mouse::Button::Right)
			{ // we want to split item
				if (this->_movingItem == nullptr)
				{
					const auto hovered_item = getHoveredItem(mouse_pos);

					if (hovered_item != nullptr)
					{
						this->_playerInvComponent.splitItem(hovered_item->cords);
					}
				}
				else
				{
					if (this->_movingItem->item.curr_num_of_blocks_in_stack > 1)
					{
						const auto& slot_pos = this->mouseToSlot(mouse_pos);

						if (slot_pos != INVALID_POS)
						{
							this->_playerInvComponent.addItemToSlot(std::move(this->_movingItem->item.getCopy()), this->mouseToSlot(mouse_pos));
							this->_playerInvComponent.removeFromItem(this->_movingItem->cords);
						}
					}
				}
				this->refresh();
			}
		}
	}

	void PlayerInventory::update(float deltaTime)
	{
		if (this->isActive())
		{ // inventory opened
			for (auto& ui_item : this->_inventoryItems)
			{
				ui_item.update(deltaTime, this->_data->window);
			}
		}
		else
		{ // only hud

		}

		// for both open and closed
		if (this->_playerInvComponent.itemAddedRecently())
		{
			this->refresh();

			this->_playerInvComponent.setAddedRecently(false);
		}
	}

	void PlayerInventory::refresh()
	{
		this->refreshItemsSprites();
		this->refreshHandledItem();
	}

	void PlayerInventory::render() const
	{
		if (this->isActive())
		{ // inventory opened
			this->_data->window.draw(*this->_background); // renders background

			// render items
			for (const auto& ui_item : this->_inventoryItems)
			{
				ui_item.render(this->_data->window);
			}
		}
		else
		{ // render only first 7 slots (hud)
			this->_data->window.draw(*this->_hud);

			for (int i = 0; i < 7; i++)
			{
				if (i >= (int)this->_inventoryItems.size())
					break;

				auto& ui_item = this->_inventoryItems[i];
				// find item with needed position
				for (int j = 0; j < 7; j++)
				{
					if (ui_item.cords.y == 0 && ui_item.cords.x == j)
					{
						ui_item.render(this->_data->window);
					}
				}
			}

			// hud selector render
			this->_data->window.draw(*this->_hudSelector->shape);
		}
	}

	void PlayerInventory::open()
	{
		this->setActive(true);
		this->refresh();
	}

	void PlayerInventory::close()
	{
		this->setActive(false);

		if (this->_movingItem != nullptr)
		{
			this->_movingItem->following_mouse = false;
			this->_movingItem = nullptr;
		}

		this->refresh();

		this->refreshHud();
	}
}
