#pragma once

#include "engine/ecs/ECS.h"
#include "engine/ecs/Entity.h"
#include "engine/ecs/components/SpriteComponent.h"

namespace Engine
{
	class AnimationComponent
		: public Component
	{
	private:
		class Animation
		{
		private:
			SpriteComponent& _sprComponent;
			
			const sf::IntRect _start;
			const sf::IntRect _end;
			sf::IntRect _current;

			sf::Vector2i _size;

			float _timer;
			float _speed;
			float _delay;

		public:
			Animation(SpriteComponent& sprComponent, float speed, float delay, sf::Vector2u start_pos, sf::Vector2u end_pos, sf::Vector2u size)
				:_sprComponent(sprComponent),
				_start(sf::IntRect(start_pos.x * size.x, start_pos.y * size.y, size.x, size.y)), 
				_end(sf::IntRect(end_pos.x * size.x, end_pos.y * size.y, size.x, size.y)), _current(_start),
				_speed(speed), _timer(0.f), _delay(delay), _size(size)
			{
				this->_sprComponent.getSpr().setTextureRect(this->_start);
			}

			void update(float deltaTime)
			{
				// timer logic
				this->_timer += this->_speed * deltaTime;
				if (this->_timer >= _delay)
				{
					this->_timer = 0.f;

					// if we haven't reached the ned yet
					if (this->_current != this->_end)
					{
						this->_current.left += this->_size.x;
					}
					else // set current frame to start
					{
						this->_current = this->_start;
					}
					this->_sprComponent.getSpr().setTextureRect(this->_current); // update sprite texture
				}
			}

			void reset()
			{
				this->_timer = 0.f;
				this->_current = this->_start;
			}
		};

		// animations data
		SpriteComponent& _sprComponent;
		std::map<std::string, Animation*> _animations; // all animations
		Animation* _lastAnimation; // last played animation

	public:
		AnimationComponent(Entity* entity, SpriteComponent& sprComponent);
		~AnimationComponent();

		void add(std::string name, float speed, float delay, 
			sf::Vector2u start_pos, sf::Vector2u end_pos, sf::Vector2u size);
		void update(std::string animation_name, float deltaTime);

	};
}
