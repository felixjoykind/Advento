#pragma once

#include "State.h"
#include "MainMenuState.h"
#include "GenerationState.h"

#include "engine/ui/elements/Scroller.h"
#include "engine/ui/elements/Button.h"
#include "engine/ui/elements/WorldPlate.h"

class SavesState :
    public State
{
private:
    GameDataRef _data;

    // background
    sf::Sprite* _background;

    UI::Scroller<UI::WorldPlate>* _scroller; // worlds list scroller
    std::vector<UI::WorldPlate*> _worlds;
    std::map<std::string, UI::Button*> _buttons; // buttons container

public:
    SavesState(GameDataRef data);
    ~SavesState();

    void Init() override;

    void HandleInput() override;

    void Update(float deltaTime) override;
    void Render() const override;

    void RefreshWorldsList();

    void Resume() override;

};

