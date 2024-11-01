#pragma once

#include <cstdint>
#include <utility>
#include <WString.h>

struct Output {

    void clearLights() {
        litMask = 0;
    }

    void allLights() {
        litMask = (1 << BUTTON_LAST) - 1;
    }

    void setLight(Button button) {
        litMask = litMask | (1 << button);
    }

    bool checkLight(Button button) const {
        return litMask & (1 << button);
    }

    void resetLight(Button button) {
        litMask = litMask & ~(1 << button);
    }

    unsigned int numberOfLitButtons() const {
        unsigned int lit = 0;
        for (Button button = BUTTON_1; button <= BUTTON_9; button = Button(button + 1)) {
            if (checkLight(button)) {
                lit++;
            }
        }
        return lit;
    }

    unsigned int litMask = 0;
    String displayText = "";

};
