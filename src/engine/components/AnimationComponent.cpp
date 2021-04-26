#include "AnimationComponent.h"

namespace Engine
{
    AnimationComponent::AnimationComponent(sf::Sprite& sprite, const sf::Texture& texture)
        :_spr(sprite), _texture(texture), _lastAnimation(nullptr)
    { }

    AnimationComponent::~AnimationComponent()
    {
        for (const auto& [name, animation] : _animations)
            delete animation;
    }

    void AnimationComponent::Add(std::string name, float speed, float delay, 
        sf::Vector2i start_pos, sf::Vector2i end_pos, sf::Vector2i size)
    {
        // adding new animation with custom parameters
        this->_animations[name] = new Animation(this->_spr, this->_texture, 
            speed, delay, start_pos, end_pos, size);
    }

    void AnimationComponent::Update(std::string animation_name, float deltaTime)
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
