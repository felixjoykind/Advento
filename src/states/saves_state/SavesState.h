#pragma once

#include "states/State.h"
#include "states/MainMenuState.h"
#include "states/GenerationState.h"

#include "engine/ui/elements/Scroller.h"
#include "engine/ui/elements/Button.h"
#include "engine/ui/elements/WorldPlate.h"

#include "UI_DEFENITIONS.h" // including current state ui defenitions

class SavesState :
    public State
{
private:
    GameDataRef _data;

    // background
    sf::Sprite* _background;

    UI::Scroller<UI::WorldPlate>* _scroller; // worlds list scroller

    // ui containers
    std::vector<UI::WorldPlate*> _worlds;
    std::map<std::string, sf::Text*> _titles;
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

