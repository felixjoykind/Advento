#include "MainMenuState.h"

MainMenuState::MainMenuState(GameDataRef data)
	:_data(data)
{
}

MainMenuState::~MainMenuState()
{
	for (auto const& [name, button] : this->_buttons)
		delete button;
}

void MainMenuState::Init()
{
	// init buttons
	this->_buttons["PLAY"] = new UI::Button(_data, 
		{ float(_data->winConfig.width / 2 - 125), float(_data->winConfig.height / 2 - 150) }, { 250.f, 60.f }, // position and size
		sf::Color(141, 153, 174, 0), sf::Color(141, 153, 174, 0), sf::Color(141, 153, 174, 0), // background colors
		sf::Color(255, 255, 255, 180), sf::Color(255, 255, 255), sf::Color(255, 255, 255, 100), // text colors
		sf::Text("New Game", _data->assets.GetFont("menu font"), 46)); // content and font

	this->_buttons["SETTINGS"] = new UI::Button(_data,
		{ float(_data->winConfig.width / 2 - 125), float(_data->winConfig.height - 370) }, { 250.f, 60.f },
		sf::Color(141, 153, 174, 0), sf::Color(141, 153, 174, 0), sf::Color(141, 153, 174, 0),
		sf::Color(255, 255, 255, 180), sf::Color(255, 255, 255), sf::Color(255, 255, 255, 100),
		sf::Text("Settings", _data->assets.GetFont("menu font"), 46)); 

	this->_buttons["QUIT"] = new UI::Button(_data, 
		{ float(_data->winConfig.width / 2 - 70), float(_data->winConfig.height - 300) }, { 140.f, 60.f },
		sf::Color(141, 153, 174, 0), sf::Color(141, 153, 174, 0), sf::Color(141, 153, 174, 0),
		sf::Color(255, 255, 255, 180), sf::Color(255, 255, 255), sf::Color(255, 255, 255, 100),
		sf::Text("Quit", _data->assets.GetFont("menu font"), 46));
}

void MainMenuState::HandleInput()
{
	sf::Event ev;
	while (_data->window.pollEvent(ev))
	{
		if (ev.type == sf::Event::Closed)
		{
			_data->window.close();
		}
	}
}

void MainMenuState::Update(float deltaTime)
{
	if (_buttons["QUIT"]->isPressed(sf::Mouse::Left))
	{
		_data->window.close();
	}
	if (_buttons["PLAY"]->isPressed(sf::Mouse::Left))
	{
		_data->states.AddState(StateRef(new GameState(_data)), true); // start game
	}
	
	// updating all buttons
	for (const auto& [name, button] : _buttons)
		button->update(deltaTime);
}

void MainMenuState::Render() const
{
	_data->window.clear();
	// restoring default view
	this->_data->window.setView(_data->window.getDefaultView());

	// rendering all buttons
	for (auto const& [name, button] : _buttons)
		button->render();

	_data->window.display();
}
