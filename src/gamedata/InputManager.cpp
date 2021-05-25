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
