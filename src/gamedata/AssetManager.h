#pragma once

#include <SFML/Graphics.hpp>

class AssetManager
{
private:
	std::map<std::string, sf::Texture> _textures;
	std::map<std::string, sf::Font> _fonts;

public:
	AssetManager();
	~AssetManager();

	// Setters
	void AddTexture(std::string name, std::string filepath);
	void AddFont(std::string name, std::string filepath);

	// Getters
	const sf::Texture& GetTexture(std::string name) const;
	const sf::Font& GetFont(std::string name) const;
};

