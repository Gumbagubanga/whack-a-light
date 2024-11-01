#pragma once

class GameEngineClass {
public:
    GameEngineClass() = default;

    ~GameEngineClass() = default;

    void init();

    void loop();

};

extern GameEngineClass GameEngine;