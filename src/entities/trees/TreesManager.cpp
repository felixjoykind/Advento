#include "TreesManager.h"

TreesManager::TreesManager()
{ }

TreesManager::~TreesManager()
{ }

void TreesManager::addTree(const Advento::Tree* tree)
{
	this->_trees.emplace_back(tree);
}
