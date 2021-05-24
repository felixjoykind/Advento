#pragma once

#include "State.h"
#include "MainMenuState.h"
#include "GenerationState.h"
#include "engine/ui/Button.h"
#include "engine/ui/WorldPlate.h"

class SavesState :
    public State
{
private:
    GameDataRef _data;

    // background
    sf::Sprite* _background;

    std::vector<UI::WorldPlate*> _worlds;
    std::map<std::string, UI::Button*> _buttons; // buttons container

public:
    SavesState(GameDataRef data);
    ~SavesState();

    void Init() override;

    void HandleInput() override;

    void Update(float deltaTime) override;
    void Render() const override;

};

