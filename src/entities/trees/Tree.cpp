#include "Tree.h"

#include "engine/ecs/components/PositionComponent.h"
#include "engine/ecs/components/SpriteComponent.h"
#include "engine/ecs/components/HealthbarComponent.h"

namespace Advento
{
	Tree::Tree(GameDataRef data, sf::Vector2f pos)
		:Entity(data, pos)
	{
		// creating components
		using namespace Engine;

		this->addComponent<PositionComponent,
			Entity*, float, float>(this, 0.f, 0.f);
		this->getComponent<PositionComponent>().setPosition(pos.x, pos.y);

		this->addComponent<SpriteComponent,
			Entity*, const sf::Texture&>(this, _data->assets.GetTexture("tree tile"));

		this->addComponent<HealthbarComponent,
			Entity*, float, float>(this, 100.f, 100.f);
	}

	Tree::~Tree()
	{ }

	void Tree::update(float deltaTime)
	{
		Entity::update(deltaTime);
	}

	void Tree::render() const
	{
		Entity::render();
	}
}
