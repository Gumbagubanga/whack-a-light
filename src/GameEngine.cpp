#include "GameEngine.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "OutputManager.h"
#include "Input.h"
#include "Scene.h"

GameEngineClass GameEngine;

InputManager *inputManager;
SceneManager *sceneManager;
OutputManager *outputManager;

/**
 * Load default configuration (Pins, etc)
 * Init SceneManager
 * Init Input/Outputs
 */
void GameEngineClass::init() {
    inputManager = new InputManager();
    sceneManager = new SceneManager();
    outputManager = new OutputManager();
}

void GameEngineClass::loop() {
    Input input = inputManager->processInput();
    const Output &output = sceneManager->update(input);
    outputManager->render(output);
}

