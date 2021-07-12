#include "WorldPlate.h"

namespace UI
{
	WorldPlate::WorldPlate(GameDataRef data, Engine::WorldSaveSettings settings,
		sf::Vector2f size, sf::Vector2f pos, sf::Color backgroundColor)
		:UIElement(data, size, pos), IHoverable(this), IClickable(this, 0.15f), ISelectable(this),
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

	void WorldPlate::setPosition(sf::Vector2f position)
	{
		UIElement::setPosition(position);
		this->_title->setPosition(
			this->_shape->getPosition().x + 10.f, 
			this->_shape->getPosition().y + 10.f
		);

	}

	void WorldPlate::update(float deltaTime)
	{
		// update selection
		IHoverable::update(deltaTime, this->_data->window);
		ISelectable::update(deltaTime, this->_data->window);

		// change plate based on selection type
		this->_shape->setOutlineThickness(0.f); // idle
		if (IHoverable::_isHovered)
		{
			this->_shape->setOutlineColor(sf::Color(190, 190, 190));
			this->_shape->setOutlineThickness(2.f);
		}
		if (ISelectable::_isSelected)
		{
			this->_shape->setOutlineColor(sf::Color::White);
			this->_shape->setOutlineThickness(2.f);
		}
	}

	void WorldPlate::render() const
	{
		UIElement::render();
		this->_data->window.draw(*this->_title);
	}
}
