#include "Player.h"

Player::Player(GameDataRef data, sf::Vector2f pos = { 0.f, 0.f })
	:Entity(data, pos)
{
	using namespace Engine;

	// creating components
	this->addComponent<PositionComponent,
		Entity*, float, float>(this, 0.f, 0.f);

	this->addComponent<SpriteComponent,
		Entity*, const sf::Texture&>(this, _data->assets.GetTexture("player anim"));

	this->addComponent<MovementComponent, 
		Entity*, MovementSettings>(this, { 250.f, 20.f, 10.f });

	this->addComponent<AnimationComponent,
		Entity*, SpriteComponent&>(this, this->getComponent<SpriteComponent>());

	this->addComponent<HitboxComponent, 
		Entity*, HitboxSettings>(this, { { 0.f, 0.f }, { 64.f, 128.f } });
	
	// adding animation
	auto& animation = this->getComponent<Engine::AnimationComponent>();
	animation.add("walk_up", 10.f, 1.f, { 0, 4 }, { 2, 4 }, { 64, 128 });
	animation.add("walk_down", 10.f, 1.f, { 0, 3 }, { 2, 3 }, { 64, 128 });
	animation.add("walk_right", 10.f, 2.f, { 0, 1 }, { 1, 1 }, { 64, 128 });
	animation.add("walk_left", 10.f, 2.f, { 0, 2 }, { 1, 2 }, { 64, 128 });
	animation.add("idle", 10.f, 1.f, { 0, 0 }, { 17, 0 }, { 64, 128 });
}

Player::~Player()
{ }

void Player::update(float deltaTime)
{
	using namespace Engine;

	// auto variables for quick access
	auto& movement = this->getComponent<MovementComponent>();
	auto& animation = this->getComponent<AnimationComponent>();

	// moving in different directions based on input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		movement.move(0.f, -1.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		movement.move(-1.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		movement.move(0.f, 1.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		movement.move(1.f, 0.f);
	}
	
	// playing animation based on movement
	switch (this->getComponent<MovementComponent>().getState())
	{
	case MovementState::IDLE:
		animation.update("idle", deltaTime);
		break;
	case MovementState::LEFT:
		animation.update("walk_left", deltaTime);
		break;
	case MovementState::RIGHT:
		animation.update("walk_right", deltaTime);
		break;
	case MovementState::UP:
		animation.update("walk_up", deltaTime);
		break;
	case MovementState::DOWN:
		animation.update("walk_down", deltaTime);
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
