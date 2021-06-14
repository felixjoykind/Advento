#pragma once

#include "State.h"

#include "engine/ui/elements/Textbox.h"
#include "engine/ui/elements/Button.h"
#include "engine/ui/elements/messagebox/MessageboxManager.h"

// State for choosing world generation settings
class GenerationState :
    public State
{
private:
    GameDataRef _data;

    // background
    sf::Sprite* _background;

    // messagebox manager
    UI::MessageboxManager* _messageboxManager;

    // ui elements
    std::map<std::string, UI::Textbox*> _textboxes; // textboxes
    std::map<std::string, UI::Button*> _buttons; // buttons
    std::map<std::string, sf::Text*> _titles; // titles

public:
    GenerationState(GameDataRef data);
    ~GenerationState();

    void Init() override;
    
    void HandleInput() override;

    // basic functions
    void Update(float deltaTime) override;
    void Render() const override;

};
