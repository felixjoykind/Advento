#include "WorldPlate.h"

#include "gamedata/InputManager.h"

namespace UI
{
	WorldPlate::WorldPlate(GameDataRef data, Engine::WorldSaveSettings settings,
		sf::Vector2f size, sf::Vector2f pos, sf::Color backgroundColor)
		:UIElement(data, size, pos), IClickable(this, 0.2f),
		_title(new sf::Text(settings.name, this->_data->assets.GetFont("menu font"), 30U)),
		_settings(settings)
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

	Engine::WorldSaveSettings WorldPlate::getSettings() const
	{
		return this->_settings;
	}

	void WorldPlate::update(float deltaTime)
	{
		// choosing plate state (hovered / selected)
		if (!_selected)
		{
			if (InputManager::isElementPressed(*this, this->_data->window, sf::Mouse::Left))
			{
				this->_selected = true;
				this->_shape->setOutlineColor(sf::Color::White);
				this->_shape->setOutlineThickness(2.f);
			}
			else if (InputManager::isElementHovered(*this, this->_data->window))
			{
				this->_shape->setOutlineColor(sf::Color(190, 190, 190));
				this->_shape->setOutlineThickness(2.f);
			}
			else
			{
				this->_shape->setOutlineThickness(0.f);
			}
		}
		else if (InputManager::isMouseButtonPressed(sf::Mouse::Left)
			&& !InputManager::isElementHovered(*this, this->_data->window))
			this->_selected = false;
	}

	void WorldPlate::render() const
	{
		this->_data->window.draw(*this->_shape);
		this->_data->window.draw(*this->_title);
	}
}
