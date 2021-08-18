#include "TreesManager.h"

TreesManager::TreesManager(GameDataRef data)
	:_data(data)
{ }

TreesManager::~TreesManager()
{ }

std::unique_ptr<Advento::Tree> TreesManager::addTree(const sf::Vector2f position)
{
	Advento::Tree* t = new Advento::Tree(_data, position);
	std::unique_ptr<Advento::Tree> tPtr{ t };
	this->_trees.emplace_back(std::move(tPtr));
	return tPtr;
}

void TreesManager::update(float deltaTime)
{
	// update all of the trees
	for (auto& t : this->_trees)
		t->update(deltaTime);
}

void TreesManager::render() const
{
	// render all of the trees
	for (const auto& t : this->_trees)
		t->render();
}
