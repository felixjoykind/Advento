#pragma once

#include "Game.h"
#include "State.h"

#include "engine/ui/elements/Textbox.h"

// State for choosing world generation settings
class GenerationState :
    public State
{
private:
    GameDataRef _data;

    // ui
    UI::Textbox* _seedTextBox;

public:
    GenerationState(GameDataRef data);
    ~GenerationState();

    void Init() override;
    
    void HandleInput() override;

    // basic functions
    void Update(float deltaTime) override;
    void Render() const override;

};
