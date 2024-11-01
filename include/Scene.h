#pragma once

#include "Input.h"
#include "Output.h"
#include <Arduino.h>

enum GameState {
    DEBUG,
    PREPARING,
    READY,
    RUNNING,
    FINISHED
};

struct Scene {

    void update(Input input) {
        elapsedTime += millis() - lastTime;

        switch (state) {
            case PREPARING:
                preparingState(input);
                break;
            case READY:
                readyState();
                break;
            case RUNNING:
                runningState(input);
                break;
            case FINISHED:
                finishedState(input);
                break;
            case DEBUG:
                debugState(input);
                break;
            default:
                break;
        }

        lastTime = millis();
    }

    Output getOutput() {
        return output;
    }

private:
    void preparingState(Input input) {
        score = 0;
        output.setLight(BUTTON_7);
        output.displayText = "Whack-a-Light\nv1.0";
        if (input.buttonReleased(BUTTON_7)) {
            switchToState(READY);
        }
    }

    void readyState() {
        output.clearLights();
        long remainingTimeInMillis = 3000L - elapsedTime;

        if (remainingTimeInMillis <= 0) {
            switchToState(RUNNING);
            return;
        }

        unsigned long remainingSeconds = divideByThousandWithCeil(remainingTimeInMillis);
        output.displayText = String("Game starts in ") + remainingSeconds;
    }

    void runningState(Input input) {
        long remainingTimeInMillis = roundTimeInMillis - elapsedTime;

        if (remainingTimeInMillis <= 0L) {
            output.clearLights();
            switchToState(FINISHED);
            return;
        }

        unsigned long remainingSeconds = divideByThousandWithCeil(remainingTimeInMillis);

        char text[34];
        sprintf(text, "Time Left %0lu\nScore %d", remainingSeconds, score);
        output.displayText = String(text);

        for (Button button = BUTTON_1; button < BUTTON_LAST; button = Button(button + 1)) {
            bool pressed = input.buttonReleased(button);
            if (!pressed) {
                continue;
            }

            if (output.checkLight(button)) {
                output.resetLight(button);
                score++;
            } else {
                if (score > 0) {
                    score--;
                }
            }
        }

        unsigned int numberOfLitButtons = output.numberOfLitButtons();
        if (numberOfLitButtons < maxNumberOfSimultaneousLights) {
            auto button = static_cast<Button>(random(BUTTON_1, BUTTON_LAST));
            output.setLight(button);
        }
    }

    void finishedState(Input input) {
        output.displayText = String("Game Over\nFinal Score ") + score;

        if (elapsedTime >= 5000) {
            output.clearLights();
            switchToState(PREPARING);
            elapsedTime = 0;
            return;
        }

        unsigned int blinkInterval = elapsedTime / blinkTime;

        if (blinkInterval <= 6) {
            if (blinkInterval % 2 == 0) {
                output.allLights();
            } else {
                output.clearLights();
            }
        }
    }

    void debugState(const Input &input) {
        output.clearLights();
        for (Button button = BUTTON_1; button < BUTTON_LAST; button = Button(button + 1)) {
            if (input.buttonPressed(button)) {
                Serial.println(String("Button pressed ") + button);
                output.setLight(button);
            }
            if (input.buttonReleased(button)) {
                Serial.println(String("Button released ") + button);
            }
        }
    }

    void switchToState(GameState gameState) {
        state = gameState;
        elapsedTime = 0;
        output.displayText.clear();
    }

    // ceil function: x/y + (x % y != 0)
    static unsigned long divideByThousandWithCeil(unsigned long dividend) {
        return (dividend / 1000) + (dividend % 1000 != 0);
    }

    const unsigned long roundTimeInMillis = 30 * 1000;
    const unsigned int maxNumberOfSimultaneousLights = 3;
    const unsigned int blinkTime = 200;

    GameState state = PREPARING;
    unsigned long lastTime = millis();
    unsigned long elapsedTime = 0L;
    unsigned int score = 0;
    Output output = Output();
};

