#pragma once

#include "engine/items/Item.h"
#include "engine/LOG.h"

constexpr float INVENTORY_OFFSET_X = 10.f;
constexpr float INVENTORY_OFFSET_Y = 50.f;

constexpr float X_GAP_BETWEEN_SLOTS = 10.f;
constexpr float Y_GAP_BETWEEN_SLOTS = 10.f;

constexpr float SLOT_SIZE = 64.f + X_GAP_BETWEEN_SLOTS + Y_GAP_BETWEEN_SLOTS;

constexpr short COLS = 7;
constexpr short ROWS = 6;

namespace Engine
{
	class Component;

	template<int inv_size>
	class InventoryComponent :
		public Component
	{
	private:
		std::array<Item, inv_size> _items; // items container

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
		InventoryComponent(Entity* entity);
		~InventoryComponent();

		const std::array<Item, inv_size>& getAllItems();

		bool addItem(Item&& item);

		void update(float deltaTime) override;

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
	inline InventoryComponent<inv_size>::InventoryComponent(Entity* entity)
		:Component(entity)
	{
	}

	template<int inv_size>
	inline InventoryComponent<inv_size>::~InventoryComponent()
	{
	}

	template<int inv_size>
	inline const std::array<Item, inv_size>& InventoryComponent<inv_size>::getAllItems()
	{
		return this->_items;
	}

	template<int inv_size>
	inline bool InventoryComponent<inv_size>::addItem(Item&& item)
	{
		bool was_possible_to_add_to_existing_stack = add_item_to_existing_stack_if_possible(item);

		if (!was_possible_to_add_to_existing_stack)
		{
			return add_item_to_new_stack_if_possible(std::move(item));
		}

		return false;
	}

	template<int inv_size>
	inline void InventoryComponent<inv_size>::update(float deltaTime)
	{
		/*system("cls");
		print();*/
	}
}
