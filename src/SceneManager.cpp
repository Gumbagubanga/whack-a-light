#include "SceneManager.h"

Output SceneManager::update(Input input) {
    currentScene.update(input);

    return currentScene.getOutput();
}
