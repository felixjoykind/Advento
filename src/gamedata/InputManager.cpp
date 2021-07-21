#include "InputManager.h"

sf::Vector2i InputManager::getMousePosition(const sf::Window& window)
{
    return sf::Mouse::getPosition(window);
}

bool InputManager::isMouseButtonPressed(sf::Mouse::Button button)
{
    return sf::Mouse::isButtonPressed(button);
}

bool InputManager::isElementHovered(UI::UIElement* element, const sf::Window& window)
{
    return element->getShape().getGlobalBounds().contains(
        { 
            float(getMousePosition(window).x), 
            float(getMousePosition(window).y)
        }
    );
}

bool InputManager::isElementPressed(UI::UIElement* element, const sf::Window& window, sf::Mouse::Button button)
{
    return isElementHovered(element, window)
        && isMouseButtonPressed(button);
}

bool InputManager::isSpriteHovered(const sf::Sprite* sprite, const sf::Window& window)
{
    auto mouse_pos = getMousePosition(window);
    return sprite->getGlobalBounds().contains((float)mouse_pos.x, (float)mouse_pos.y);
}
