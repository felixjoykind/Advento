#pragma once

#include "Game.h"
#include "State.h"

class GenerationState :
    public State
{
private:
    GameDataRef _data;

public:
    GenerationState(GameDataRef data);
    ~GenerationState();

    void Init() override;
    
    void HandleInput() override;

    // basic functions
    void Update(float deltaTime) override;
    void Render() const override;

};
