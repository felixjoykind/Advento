#include "WorldPlate.h"

#include "gamedata/InputManager.h"

namespace UI
{
	WorldPlate::WorldPlate(GameDataRef data, std::string title, 
		sf::Vector2f size, sf::Vector2f pos, sf::Color backgroundColor)
		:UIElement(data, size, pos), 
		_title(new sf::Text(title, this->_data->assets.GetFont("menu font"), 30U))
	{
		// init title
		this->_title->setPosition(_shape->getPosition().x + 10.f, _shape->getPosition().y + 10.f);

		// init background colors
		this->_shape->setFillColor(backgroundColor);
		this->_shape->setOutlineColor(sf::Color::White);
	}

	UI::WorldPlate::~WorldPlate()
	{
		delete this->_title;
	}

	void WorldPlate::update(float deltaTime)
	{
		if (InputManager::isElementHovered(*this, this->_data->window))
		{
			this->_shape->setOutlineThickness(5.f);
		}
		else
		{
			this->_shape->setOutlineThickness(0.f);
		}
	}

	void WorldPlate::render() const
	{
		this->_data->window.draw(*this->_shape);
		this->_data->window.draw(*this->_title);
	}
}
