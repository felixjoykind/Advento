#include "AnimationComponent.h"

namespace Engine
{
    AnimationComponent::AnimationComponent(Entity* entity, const sf::Texture& texture)
        :Component(entity), _texture(texture), _lastAnimation(nullptr)
    { }

    AnimationComponent::~AnimationComponent()
    {
        for (const auto& [name, animation] : _animations)
            delete animation;
    }

    void AnimationComponent::add(std::string name, float speed, float delay, 
        sf::Vector2i start_pos, sf::Vector2i end_pos, sf::Vector2i size)
    {
        // adding new animation with custom parameters
        this->_animations[name] = new Animation(this->_entity->getSpr(), this->_texture, 
            speed, delay, start_pos, end_pos, size);
    }

    void AnimationComponent::update(std::string animation_name, float deltaTime)
    {
        if (this->_lastAnimation != _animations.at(animation_name))
        {
            if (this->_lastAnimation != nullptr)
            {
                this->_lastAnimation->reset();
            }
            this->_lastAnimation = _animations.at(animation_name);
        }

        this->_animations.at(animation_name)->update(deltaTime);
    }
}
