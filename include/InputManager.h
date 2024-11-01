#pragma once

#include "Input.h"
#include <cstdint>

// You will need to change the "int" to "long" If the NUMBER_OF_SHIFT_CHIPS is higher than 2.
#define BYTES_VAL_T uint16_t

BYTES_VAL_T shiftIn165();

class InputManager {
public:
    InputManager();

    Input processInput();

private:
    static uint16_t map(uint16_t input);

    static void moveBit(uint16_t &copy, int from, int to);

    uint16_t lastInputValue = 0;
};


