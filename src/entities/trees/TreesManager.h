#pragma once

#include "Tree.h"

class TreesManager
{
private:
	GameDataRef _data;
	std::vector<std::unique_ptr<Advento::Tree>> _trees;

public:
	TreesManager(GameDataRef data);
	~TreesManager();

	std::unique_ptr<Advento::Tree> addTree(const sf::Vector2f position);
	
	// TODO: add posibility to remove trees

	void update(float deltaTime);
	void render() const;

};

