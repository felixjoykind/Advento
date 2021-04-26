#include "PauseMenu.h"

#include "states/MainMenuState.h"

PauseMenu::PauseMenu(GameDataRef data)
	:_data(data), _active(true), _title(new sf::Text("Pause", _data->assets.GetFont("menu font"), 64)),
	_background({ (float)_data->winConfig.width, (float)_data->winConfig.height }),
	_panel({ 400.f, (float)_data->winConfig.height })
{
	// init bacground and panel
	this->_background.setFillColor(sf::Color(0, 0, 0, 125));// semi transparent background
	this->_panel.setPosition(float(_data->winConfig.width / 2) - _panel.getSize().x / 2.f, 0.f);
	this->_panel.setFillColor(sf::Color(0, 0, 0, 125));// semi transparent panel

	// init texts
	this->_title->setPosition(
		float(_data->winConfig.width / 2) - _title->getGlobalBounds().width / 2.f,
		100.f
	);
	
	// init buttons
	this->_buttons["RESUME"] = new UI::Button(_data, 
		{ float(_data->winConfig.width / 2 - 100), float(_data->winConfig.height / 2 - 150) }, { 200.f, 60.f },
		sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0),
		sf::Color(255, 255, 255, 180), sf::Color(255, 255, 255), sf::Color(255, 255, 255, 100),
		sf::Text("Resume", _data->assets.GetFont("menu font"), 46)
	);

	this->_buttons["MENU"] = new UI::Button(_data,
		{ float(_data->winConfig.width / 2 - 100), float(_data->winConfig.height / 2 - 50) }, { 200.f, 60.f },
		sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0),
		sf::Color(255, 255, 255, 180), sf::Color(255, 255, 255), sf::Color(255, 255, 255, 100),
		sf::Text("Resume", _data->assets.GetFont("menu font"), 46)
	);

	this->_buttons["QUIT"] = new UI::Button(_data,
		{ float(_data->winConfig.width / 2 - 70), float(_data->winConfig.height - 300) }, { 140.f, 60.f },
		sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0),
		sf::Color(255, 255, 255, 180), sf::Color(255, 255, 255), sf::Color(255, 255, 255, 100),
		sf::Text("Quit", _data->assets.GetFont("menu font"), 46)
	);
}

PauseMenu::~PauseMenu()
{

}

bool PauseMenu::isActive() const
{
	return this->_active;
}

void PauseMenu::setActive(bool value)
{
	this->_active = value;
}

void PauseMenu::update(float deltaTime)
{
	if (_buttons["RESUME"]->isPressed(sf::Mouse::Button::Left))
	{
		this->_active = false;
	}

	if (_buttons["MENU"]->isPressed(sf::Mouse::Left))
	{
		this->_data->states.AddState(StateRef(new MainMenuState(_data)), true);
	}

	if (_buttons["QUIT"]->isPressed(sf::Mouse::Left))
	{
		this->_data->window.close();
	}


	// updating buttons
	for (auto& [name, b] : _buttons)
	{
		b->update(deltaTime);
	}
}

void PauseMenu::render() const
{
	// background and panel
	_data->window.draw(_background);
	_data->window.draw(_panel);

	// rendering title
	_data->window.draw(*this->_title);
	
	// rendering buttons
	for (const auto& [name, b] : _buttons)
		b->render();
}
