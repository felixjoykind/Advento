#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "engine/items/Item.h"
#include "engine/LOG.h"

#include "gamedata/InputManager.h"

constexpr float INVENTORY_OFFSET_X = 32.f;
constexpr float INVENTORY_OFFSET_Y = 150.f;

constexpr float X_GAP_BETWEEN_SLOTS = 16.f;
constexpr float Y_GAP_BETWEEN_SLOTS = 16.f;

constexpr float SLOT_SIZE = ITEM_SIZE + X_GAP_BETWEEN_SLOTS;

constexpr short COLS = 7;
constexpr short ROWS = 5;

constexpr int POS_INVALID_VALUE = -1;

constexpr int PLAYER_INVENTORY_SIZE = ROWS * COLS;

namespace Engine
{
	class Component;

	template<int inv_size>
	class InventoryComponent :
		public Component
	{
	private:
		std::array<Item, inv_size> _items; // items container

		const sf::View& _camera;
		const sf::RenderWindow& _window;

		// item that player handles in hand (fully controlled by UI::PlayerInventory.cpp)
		const Item* _holdedItem = nullptr;
		sf::Sprite* _holdedItemSpr = nullptr;

	private:
		bool is_slot_empty(std::size_t id) const;
		size_t find_next_empty_slot() const;
		std::vector<size_t> find_indexes_of(const Item& item) const;

		bool add_item_to_existing_stack_if_possible(const Item& item);
		bool add_item_to_new_stack_if_possible(Item&& item);

		void print() const;

	public:
		static constexpr int EMPTY_SLOT_ID = -1;

	public:
		InventoryComponent(Entity* entity, const sf::RenderWindow& window, const sf::View& view);
		~InventoryComponent();

		const std::array<Item, inv_size>& getAllItems();

		void swapItems(sf::Vector2i cords1, sf::Vector2i cords2);

		// Splits item if possible
		bool splitItem(const sf::Vector2i& item_pos);

		const Item& getItemAt(sf::Vector2i slot) const;

		bool addItemToSlot(Item&& item, sf::Vector2i slot);
		bool addToItem(sf::Vector2i slot, int amount = 1);
		bool addItem(Item&& item, bool new_slot = false);

		bool removeFromItem(sf::Vector2i slot, int amount = 1);
		bool removeItem(sf::Vector2i slot);

		bool isFull() const;

		void setHoldedItem(const Item* holdedItemconst, const AssetManager& assets);
		void removeHoldedItem();

		void update(float deltaTime) override;
		void render(sf::RenderTarget& target) const override;
	};

	template<int inv_size>
	inline bool InventoryComponent<inv_size>::is_slot_empty(std::size_t id) const
	{
		return this->_items[id].id == EMPTY_SLOT_ID;
	}

	template<int inv_size>
	inline std::vector<size_t> InventoryComponent<inv_size>::find_indexes_of(const Item& item) const
	{
		std::vector<size_t> indexes{};

		for (size_t idx = 0; idx < inv_size; ++idx)
		{
			if (this->_items[idx].id == item.id)
			{
				indexes.push_back(idx);
			}
		}

		return indexes;
	}

	template<int inv_size>
	inline size_t InventoryComponent<inv_size>::find_next_empty_slot() const
	{
		for (size_t idx = 0; idx < inv_size; ++idx)
		{
			if (is_slot_empty(idx))
			{
				return idx;
			}
		}

		return inv_size; // invalid value!
	}

	template<int inv_size>
	inline bool InventoryComponent<inv_size>::add_item_to_existing_stack_if_possible(const Item& item)
	{
		auto indexes_with_same_item = find_indexes_of(item);

		for (auto idx : indexes_with_same_item)
		{
			if (item.id == this->_items[idx].id)
			{
				if (_items[idx].curr_num_of_blocks_in_stack + item.curr_num_of_blocks_in_stack <=
					_items[idx].max_num_blocks_in_stack)
				{
					_items[idx].curr_num_of_blocks_in_stack += item.curr_num_of_blocks_in_stack;
					return true;
				}
			}
		}

		return false;
	}

	template<int inv_size>
	inline bool InventoryComponent<inv_size>::add_item_to_new_stack_if_possible(Item&& item)
	{
		size_t next_empty_slot = find_next_empty_slot();

		if (next_empty_slot >= 0)
		{
			this->_items[next_empty_slot] = std::move(item);
			return true;
		}

		return false;
	}

	template<int inv_size>
	inline void InventoryComponent<inv_size>::print() const
	{
		for (size_t i = 0; i < inv_size; ++i)
		{
			if (this->_items[i].id != EMPTY_SLOT_ID)
			{
				std::cout << "Inventory slot: " << i << "\n"
					<< "Item ID: " << this->_items[i].id << "\n"
					<< "Item Num: " << this->_items[i].curr_num_of_blocks_in_stack << "\n"
					<< "Item Max Num: " << this->_items[i].max_num_blocks_in_stack << std::endl;
			}
		}
	}

	template<int inv_size>
	inline InventoryComponent<inv_size>::InventoryComponent(Entity* entity, const sf::RenderWindow& window, const sf::View& view)
		:Component(entity), _window(window), _camera(view)
	{
		
	}

	template<int inv_size>
	inline InventoryComponent<inv_size>::~InventoryComponent()
	{
		delete this->_holdedItemSpr;
	}

	template<int inv_size>
	inline const std::array<Item, inv_size>& InventoryComponent<inv_size>::getAllItems()
	{
		return this->_items;
	}

	template<int inv_size>
	inline void InventoryComponent<inv_size>::swapItems(sf::Vector2i cords1, sf::Vector2i cords2)
	{
		int index1 = cords1.y * COLS + cords1.x;
		int index2 = cords2.y * COLS + cords2.x;
		std::swap(this->_items[index1], this->_items[index2]);
	}

	template<int inv_size>
	inline bool InventoryComponent<inv_size>::addItem(Item&& item, bool new_slot)
	{
		if (new_slot)
		{
			return add_item_to_new_stack_if_possible(std::move(item));
		}

		bool was_possible_to_add_to_existing_stack = add_item_to_existing_stack_if_possible(item);

		if (!was_possible_to_add_to_existing_stack)
		{
			return add_item_to_new_stack_if_possible(std::move(item));
		}

		return false;
	}

	template<int inv_size>
	inline bool InventoryComponent<inv_size>::removeFromItem(sf::Vector2i slot, int amount)
	{
		int i = slot.y * COLS + slot.x;
		if (this->_items[i].curr_num_of_blocks_in_stack - amount > 0)
		{
			this->_items[i].curr_num_of_blocks_in_stack -= amount;
			return true;
		}
		return false;
	}

	template<int inv_size>
	inline bool InventoryComponent<inv_size>::removeItem(sf::Vector2i slot)
	{
		int i = slot.y * COLS + slot.x;
		if (this->_items[i].id != EMPTY_SLOT_ID)
		{
			this->_items[i] = Item{}; // replace with empty item
			return true;
		}
		return false;
	}

	template<int inv_size>
	inline bool InventoryComponent<inv_size>::isFull() const
	{
		for (const auto& item : this->_items)
		{
			if (item.id == EMPTY_SLOT_ID)
				return false;
		}
		return true;
	}

	template<int inv_size>
	inline void InventoryComponent<inv_size>::setHoldedItem(const Item* holdedItem, const AssetManager& assets)
	{
		this->_holdedItem = holdedItem;
		this->_holdedItemSpr = new sf::Sprite(this->_holdedItem->getTexture(assets));

		if (this->_entity->hasComponent<PositionComponent>())
		{
			this->_holdedItemSpr->setOrigin(
				this->_holdedItemSpr->getPosition().x,
				this->_holdedItemSpr->getPosition().y + _holdedItemSpr->getTexture()->getSize().y
			);

			this->_holdedItemSpr->setPosition(
				{
					this->_entity->getComponent<PositionComponent>().getX() + 60.f,
					this->_entity->getComponent<PositionComponent>().getY() + 84.f
				}
			);
		}
	}

	template<int inv_size>
	inline void InventoryComponent<inv_size>::removeHoldedItem()
	{
		this->_holdedItem = nullptr;
		delete this->_holdedItemSpr;
		this->_holdedItemSpr = nullptr;
	}

	template<int inv_size>
	inline bool InventoryComponent<inv_size>::splitItem(const sf::Vector2i& item_pos)
	{
		if (this->isFull() == false)
		{
			auto& item = this->_items[item_pos.y * COLS + item_pos.x];

			if (item.curr_num_of_blocks_in_stack > 1)
			{
				this->addItem(std::move(item.getCopy(item.getHalfOfAmount())), true);
				item.curr_num_of_blocks_in_stack /= 2;
				return true;
			}
		}

		return false;
	}

	template<int inv_size>
	inline const Item& InventoryComponent<inv_size>::getItemAt(sf::Vector2i slot) const
	{
		return this->_items[slot.y * COLS + slot.x];
	}

	template<int inv_size>
	inline bool InventoryComponent<inv_size>::addItemToSlot(Item&& item, sf::Vector2i slot)
	{
		int i = slot.y * COLS + slot.x;
		if (this->_items[i].id == EMPTY_SLOT_ID)
		{
			this->_items[i] = std::move(item);
			return true;
		}
		else if (this->_items[i].id == item.id)
		{
			return this->addToItem(slot, item.curr_num_of_blocks_in_stack);
		}
		return false;
	}

	template<int inv_size>
	inline bool InventoryComponent<inv_size>::addToItem(sf::Vector2i slot, int amount)
	{
		auto& item = this->_items[slot.y * COLS + slot.x];
		if (item.curr_num_of_blocks_in_stack + amount <= item.max_num_blocks_in_stack)
		{
			item.curr_num_of_blocks_in_stack += amount;
			return true;
		}
		return false;
	}

	template<int inv_size>
	inline void InventoryComponent<inv_size>::update(float deltaTime)
	{
		/*system("cls");
		print();*/

		if (this->_holdedItemSpr != nullptr && this->_entity->hasComponent<PositionComponent>())
		{ // setting position and rotation
			sf::Vector2i hold_item_pos_screen = this->_window.mapCoordsToPixel(_holdedItemSpr->getPosition(), this->_camera);
			sf::Vector2f item_pos = this->_entity->getComponent<PositionComponent>().getPosition();
			item_pos.y = item_pos.y + 84.f;

			// set rotation
			auto mouse_pos = InputManager::getMousePosition(this->_window);
			sf::Vector2f diff = {
				float(mouse_pos.x - hold_item_pos_screen.x),
				float(mouse_pos.y - hold_item_pos_screen.y)
			};

			float rotation = atan2(diff.y, diff.x) * float(180) / float(M_PI);

			if (this->_entity->hasComponent<MovementComponent>())
			{ // set position in hand
				switch (this->_entity->getComponent<MovementComponent>().getState())
				{
				case MovementState::RIGHT:
					item_pos.x = item_pos.x + 50.f;
					this->_holdedItemSpr->setScale(1.f, 1.f);
					rotation += 45.f;
					break;
				case MovementState::LEFT:
					item_pos.x = item_pos.x + 10.f;
					this->_holdedItemSpr->setScale(-1.f, 1.f);
					rotation += 135.f;
					break;
				case MovementState::IDLE:
				case MovementState::UP:
				case MovementState::DOWN:
					item_pos.x = item_pos.x + 60.f;
					this->_holdedItemSpr->setScale(1.f, 1.f);
					rotation += 45.f;
					break;
				}
			}
			else
			{
				item_pos.x = item_pos.x + 60.f;
				rotation += 45.f;
			}

			this->_holdedItemSpr->setPosition(item_pos);
			this->_holdedItemSpr->setRotation(rotation);
		}

	}

	template<int inv_size>
	inline void InventoryComponent<inv_size>::render(sf::RenderTarget& target) const
	{
		if (this->_holdedItemSpr != nullptr)
		{
			target.draw(*this->_holdedItemSpr);
		}
	}
}
