#include "AnimationComponent.h"

namespace Engine
{
    AnimationComponent::AnimationComponent(Entity* entity, SpriteComponent& sprComponent)
        :Component(entity), _sprComponent(sprComponent), _lastAnimation(nullptr)
    { }

    AnimationComponent::~AnimationComponent()
    {
        for (const auto& [name, animation] : _animations)
            delete animation;
    }

    void AnimationComponent::add(std::string name, float speed, float delay, 
        sf::Vector2u start_pos, sf::Vector2u end_pos, sf::Vector2u size)
    {
        // adding new animation with custom parameters
        this->_animations[name] = new Animation(this->_sprComponent, 
            speed, delay, start_pos, end_pos, size);
    }

    void AnimationComponent::update(std::string animation_name, float deltaTime)
    {
        // if new animation is not old then set _lastAnimation
        if (this->_lastAnimation != _animations.at(animation_name))
        {
            if (this->_lastAnimation != nullptr) // reset if != nullptr
            {
                this->_lastAnimation->reset();
            }
            // update last animation
            this->_lastAnimation = _animations.at(animation_name);
        }

        this->_animations.at(animation_name)->update(deltaTime);
    }
}
