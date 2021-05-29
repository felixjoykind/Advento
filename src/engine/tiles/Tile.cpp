#include "Tile.h"

#include "engine/defenitions/BASIC_WORLD_SETTINGS.h"

namespace Engine
{
	Tile::Tile(const sf::Texture& texture, sf::Vector2f pos)
		:_spr(new sf::Sprite(texture))
	{
		this->_spr->setPosition(pos);
	}

	Tile::~Tile()
	{
		delete this->_spr;
	}

	sf::Vector2u Tile::getGridPosition() const
	{
		return sf::Vector2u(
			static_cast<unsigned>(this->_spr->getPosition().x / TILE_SIZE),
			static_cast<unsigned>(this->_spr->getPosition().y / TILE_SIZE)
		);
	}

	void Tile::update(float deltaTime)
	{
	}

	void Tile::render(sf::RenderTarget& target)
	{
		target.draw(*this->_spr);
	}
}
