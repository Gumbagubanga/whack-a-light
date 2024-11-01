#include "GameEngine.h"
#include <Arduino.h>

void setup() {
    GameEngine.init();

    Serial.begin(9600);
}

void loop() {
    GameEngine.loop();
}