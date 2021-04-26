#include "Player.h"

Player::Player(GameDataRef data, sf::Vector2f pos)
	:Entity(data, pos)
{
	// creating components
	this->loadTexture(_data->assets.GetTexture("player anim"));
	this->createMovementComponent({ 150.f, 15.f, 5.f });
	this->createAnimationComponent(*this->_spr, *this->_spr->getTexture());
	this->createHitboxComponent({ *this->_spr, { 0.f, 0.f }, { 64.f, 128.f }});

	// DEBUG
	this->_hitbox->setVisible(true);
	
	// adding animations
	this->_animation->Add("walk_up", 10.f, 1.f, { 0, 4 }, { 2, 4 }, { 64, 128 });
	this->_animation->Add("walk_down", 10.f, 1.f, { 0, 3 }, { 2, 3 }, { 64, 128 });
	this->_animation->Add("walk_right", 10.f, 2.f, { 0, 1 }, { 1, 1 }, { 64, 128 });
	this->_animation->Add("walk_left", 10.f, 2.f, { 0, 2 }, { 1, 2 }, { 64, 128 });
	this->_animation->Add("idle", 10.f, 1.f, { 0, 0 }, { 17, 0 }, { 64, 128 });
}

Player::~Player()
{ }

void Player::update(float deltaTime)
{
	// movement input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		this->move(0.f, -1.f, deltaTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		this->move(-1.f, 0.f, deltaTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		this->move(0.f, 1.f, deltaTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		this->move(1.f, 0.f, deltaTime);
	}
	
	// playing animation
	switch (this->_movement->getState())
	{
		using namespace Engine;
	case MovementState::IDLE:
		this->_animation->Update("idle", deltaTime);
		break;
	case MovementState::LEFT:
		this->_animation->Update("walk_left", deltaTime);
		break;
	case MovementState::RIGHT:
		this->_animation->Update("walk_right", deltaTime);
		break;
	case MovementState::UP:
		this->_animation->Update("walk_up", deltaTime);
		break;
	case MovementState::DOWN:
		this->_animation->Update("walk_down", deltaTime);
		break;
	}

	// base entity update function call
	Entity::update(deltaTime);
}

void Player::render() const
{
	// basic entity render function call
	Entity::render();
}
