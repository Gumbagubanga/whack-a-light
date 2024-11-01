#pragma once

#include <cstdint>

enum Button {
    BUTTON_1 = 0,
    BUTTON_2 = 1,
    BUTTON_3 = 2,
    BUTTON_4 = 3,
    BUTTON_5 = 4,
    BUTTON_6 = 5,
    BUTTON_7 = 6,
    BUTTON_8 = 7,
    BUTTON_9 = 8,
    BUTTON_LAST = BUTTON_9 + 1
};

struct Input {

    Input(uint16_t input, uint16_t lastInput) {
        this->inputValue = input;
        this->lastInputValue = lastInput;
    }

    bool buttonPressed(Button button) const {
        return inputValue & (1 << button);
    }

    bool anyButtonPressedDown() const {
        return inputValue > 0;
    }

    bool buttonReleased(Button button) const {
        bool notPressed = !(inputValue & (1 << button));
        bool beenPressed = lastInputValue & (1 << button);
        return notPressed && beenPressed;
    }

private:
    uint16_t inputValue;
    uint16_t lastInputValue;
};
