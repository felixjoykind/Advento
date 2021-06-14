#include "MessageboxManager.h"

namespace UI
{
	MessageboxManager::MessageboxManager(GameDataRef data)
		:_data(data)
	{
	}

	MessageboxManager::~MessageboxManager()
	{
	}

	void MessageboxManager::update(float deltaTime)
	{
		auto it = std::remove_if(_messageboxes.begin(), _messageboxes.end(),
			[](const std::unique_ptr<MessageBox> &m) { return m->isActive() == false; });

		// if unactive messagebox was found
		if (it != this->_messageboxes.end())
		{
			// delete it
			this->_messageboxes.erase(it);
		}

		// update boxes
		for (auto& box : this->_messageboxes)
			box->update(deltaTime);
	}

	void MessageboxManager::render() const
	{
		// render boxes
		for (const auto& box : this->_messageboxes)
			box->render();
	}

	void MessageboxManager::show(MessageBox* messagebox)
	{
		// add new messagebox
		auto ptr = std::unique_ptr<MessageBox>(messagebox);
		this->_messageboxes.push_back(std::move(ptr));
	}
}
