#pragma once

#include "Scene.h"
#include <cstdint>

// You will need to change the "int" to "long" If the NUMBER_OF_SHIFT_CHIPS is higher than 2.
#define BYTES_VAL_T uint16_t

void shiftOut595(BYTES_VAL_T value);

class OutputManager {
public:
    OutputManager();

    void render(const Output &output);

private:
    static uint16_t map(uint16_t output);

    static void setLitMask(uint16_t output, int bitPosition, int mappedPosition, uint16_t &copy);
};
