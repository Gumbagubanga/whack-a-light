#include "InputManager.h"
#include <Arduino.h>
#include <cstdint>

// How many shift register chips are daisy-chained.
#define NUMBER_OF_SHIFT_CHIPS 2

// Width of data (how many ext lines).
#define DATA_WIDTH (NUMBER_OF_SHIFT_CHIPS * 8)

// Connects to Parallel load pin the 165
#define LATCH_OUT_PIN 15
// Connects to the Clock pin the 165
#define CLOCK_OUT_PIN 2
// Connects to the Q7 pin the 165
#define DATA_OUT_PIN 4

InputManager::InputManager() {
    // Initialize our digital pins...
    pinMode(LATCH_OUT_PIN, OUTPUT);
    pinMode(CLOCK_OUT_PIN, OUTPUT);
    pinMode(DATA_OUT_PIN, INPUT);
}

Input InputManager::processInput() {
    uint16_t inputValue = shiftIn165();
    uint16_t mappedValue = map(inputValue);

    const Input &input = Input{
            mappedValue,
            lastInputValue
    };

    lastInputValue = mappedValue;
    return input;
}

uint16_t InputManager::map(const uint16_t input) {
    uint16_t copy = input;

    // 64 -> 16
    moveBit(copy, 6, 4);
    // 128 -> 32
    moveBit(copy, 7, 5);
    // 256 -> 64
    moveBit(copy, 8, 6);
    // 512 -> 128
    moveBit(copy, 9, 7);
    // 32768 -> 128
    moveBit(copy, 15, 8);

    return copy;
}

void InputManager::moveBit(uint16_t &copy, int from, int to) {
    // check: integer & (1 << bit_position_to_check)
    // set:   integer | (1 << bit_position_to_be_set)
    // clear: integer & ~(1 << bit_position_to_clear)

    if (copy & (1 << from)) {
        copy = copy | (1 << to);
        copy = copy & ~(1 << from);
    }
}

/**
 * This function is essentially a "shift-in" routine reading the
 * serial Data from the shift register chips and representing
 * the state of those pins in an unsigned integer (or long).
 * https://playground.arduino.cc/Code/ShiftRegSN74HC165N/
 * https://forum.arduino.cc/t/74hc165-shift-register-with-shiftin-wraps-last-bit-around-the-beginning/601084/12
 */
BYTES_VAL_T shiftIn165() {
    BYTES_VAL_T value = 0;

    // Trigger a parallel Load to latch the state of the data lines
    digitalWrite(LATCH_OUT_PIN, HIGH);

    // Loop to read each bit value from the serial out line of the SN74HC165N.
    for (int i = 0; i < DATA_WIDTH; i++) {
        // Set the corresponding bit in value.
        value |= digitalRead(DATA_OUT_PIN) << ((DATA_WIDTH - 1) - i);

        // Pulse the Clock (rising edge shifts the next bit).
        digitalWrite(CLOCK_OUT_PIN, HIGH);
        digitalWrite(CLOCK_OUT_PIN, LOW);
    }
    digitalWrite(LATCH_OUT_PIN, LOW);

    return value;
}