#pragma once

#include "MessageBox.h"

namespace UI
{
	// Simple class for handeling messagebox
	class MessageboxManager
	{
	private:
		GameDataRef _data; // data
		std::vector<std::unique_ptr<MessageBox>> _messageboxes; // messagebox container

	public:
		MessageboxManager(GameDataRef data);
		~MessageboxManager();

		void update(float deltaTime);
		void render() const;

		// Creates new messagebox
		void show(MessageBox* messagebox);

	};
}

