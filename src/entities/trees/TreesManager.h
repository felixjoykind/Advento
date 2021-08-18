#pragma once

#include "Tree.h"

class TreesManager
{
private:
	std::vector<Advento::Tree*> _trees;

public:
	TreesManager();
	~TreesManager();

	void addTree(const Advento::Tree* tree);
	
	// TODO: add posibility to remove trees

};

