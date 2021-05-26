#include "WorldPlate.h"

#include "gamedata/InputManager.h"

namespace UI
{
	WorldPlate::WorldPlate(GameDataRef data, Engine::WorldSaveSettings settings,
		sf::Vector2f size, sf::Vector2f pos, sf::Color backgroundColor)
		:UIElement(data, size, pos), IClickable(this, 0.2f), ISelectable(this),
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
		// update selection
		ISelectable::update(deltaTime, this->_data->window);

		// change plate based on selection type
		switch (ISelectable::getSelectionType())
		{
		case SelectionType::NONE:
			this->_shape->setOutlineThickness(0.f);
			break;
		case SelectionType::HOVERED:
			this->_shape->setOutlineColor(sf::Color(190, 190, 190));
			this->_shape->setOutlineThickness(2.f);
			break;
		case SelectionType::SELECTED:
			this->_shape->setOutlineColor(sf::Color::White);
			this->_shape->setOutlineThickness(2.f);
			break;
		}
	}

	void WorldPlate::render() const
	{
		this->_data->window.draw(*this->_shape);
		this->_data->window.draw(*this->_title);
	}
}
