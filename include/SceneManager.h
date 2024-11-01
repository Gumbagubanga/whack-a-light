#pragma once

#include "Scene.h"
#include "Input.h"
#include "Output.h"

class SceneManager {

public:
    Output update(Input input);

private:
    Scene currentScene;
};
