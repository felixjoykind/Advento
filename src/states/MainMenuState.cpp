#include "MainMenuState.h"

MainMenuState::MainMenuState(GameDataRef data)
	:_data(data), 
	_title(new sf::Text("Advento.", this->_data->assets.GetFont("menu title font"), 60U)),
	_background(new sf::Sprite())
{
}

MainMenuState::~MainMenuState()
{
	delete this->_title;
	delete this->_background;

	// delete all buttons
	for (auto const& [name, button] : this->_buttons)
		delete button;
}

void MainMenuState::Init()
{
	// restoring default view
	this->_data->window.setView(_data->window.getDefaultView());

	// init title
	this->_title->setPosition({
		float(this->_data->winConfig.width - _title->getGlobalBounds().width * 2),
		float(this->_data->winConfig.height / 2 - _title->getGlobalBounds().height / 2),
	});

	// init background
	this->_background->setTexture(this->_data->assets.GetTexture("main menu background"));
	
	// stretching background image to window size
	sf::Vector2f target_background_size
	{
		float(this->_data->winConfig.width),
		float(this->_data->winConfig.height),
	};
	this->_background->setScale(
		target_background_size.x / this->_background->getLocalBounds().width,
		target_background_size.y / this->_background->getLocalBounds().height
	);

	// init buttons
	this->_buttons["PLAY"] = new UI::Button(_data, 
		{ 10.f, float(_data->winConfig.height / 2 - 100) }, { 100.f, 60.f }, // position and size
		sf::Color(141, 153, 174, 0), sf::Color(141, 153, 174, 0), sf::Color(141, 153, 174, 0), // background colors
		sf::Color(255, 255, 255, 180), sf::Color(255, 255, 255), sf::Color(255, 255, 255, 100), // text colors
		sf::Text("Play", _data->assets.GetFont("menu font"), 46)); // content and font

	this->_buttons["SETTINGS"] = new UI::Button(_data,
		{ 10.f, float(_data->winConfig.height / 2 - 40) }, { 190.f, 60.f },
		sf::Color(141, 153, 174, 0), sf::Color(141, 153, 174, 0), sf::Color(141, 153, 174, 0),
		sf::Color(255, 255, 255, 180), sf::Color(255, 255, 255), sf::Color(255, 255, 255, 100),
		sf::Text("Settings", _data->assets.GetFont("menu font"), 46)); 

	this->_buttons["QUIT"] = new UI::Button(_data, 
		{ 10.f, float(_data->winConfig.height / 2 + 20) }, { 100.f, 60.f },
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
		_data->states.AddState(StateRef(new SavesState(_data)), false); // start game
	}
	
	// updating all buttons
	for (const auto& [name, button] : _buttons)
		button->update(deltaTime);
}

void MainMenuState::Render() const
{
	_data->window.clear();

	// render background
	this->_data->window.draw(*this->_background);

	// rednering title
	this->_data->window.draw(*this->_title);

	// rendering all buttons
	for (auto const& [name, button] : _buttons)
		button->render();

	_data->window.display();
}
