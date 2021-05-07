#include "AssetManager.h"

AssetManager::AssetManager()
{ }

AssetManager::~AssetManager()
{ }

void AssetManager::AddTexture(std::string name, std::string filepath)
{
    sf::Texture new_texture;
	if (new_texture.loadFromFile(filepath))
	{
		new_texture.setSmooth(true);
		this->_textures[name] = new_texture;
	}
	else
	{
		throw("Can't load texture from " + filepath);
	}
}

void AssetManager::AddFont(std::string name, std::string filepath)
{
	sf::Font new_font;
	if (new_font.loadFromFile(filepath))
	{
		this->_fonts[name] = new_font;
	}
	else
	{
		throw("Ca't load font from" + filepath);
	}
}

const sf::Texture& AssetManager::GetTexture(std::string name) const
{
    return this->_textures.at(name);
}

const sf::Font& AssetManager::GetFont(std::string name) const
{
	return this->_fonts.at(name);
}
