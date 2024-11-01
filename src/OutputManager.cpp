#include "OutputManager.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// How many shift register chips are daisy-chained.
#define NUMBER_OF_SHIFT_CHIPS 2

// Width of data (how many ext lines).
#define DATA_WIDTH (NUMBER_OF_SHIFT_CHIPS * 8)

// Connects to Parallel load pin the 595
#define LATCH_OUT_PIN 12
// Connects to the Clock pin the 595
#define CLOCK_OUT_PIN 14
// Connects to the Q7 pin the 595
#define DATA_OUT_PIN 13

#define I2C_SDA 21
#define I2C_SCL 22

LiquidCrystal_I2C lcd(0x27, 16, 2);

OutputManager::OutputManager() {
    // Initialize our digital pins...
    pinMode(LATCH_OUT_PIN, OUTPUT);
    pinMode(CLOCK_OUT_PIN, OUTPUT);
    pinMode(DATA_OUT_PIN, OUTPUT);

    lcd.init(I2C_SDA, I2C_SCL);
    lcd.backlight();
}

void OutputManager::render(const Output &output) {
    uint16_t litMask = output.litMask;
    uint16_t mappedLitMask = map(litMask);
    shiftOut595(mappedLitMask);

    const String &displayOutput = output.displayText;
    if (displayOutput.isEmpty()) {
        lcd.clear();
    } else {
        int indexOf = displayOutput.indexOf("\n");
        if (indexOf < 0) {
            char buffer[17];
            sprintf(buffer, "%-16s", displayOutput.c_str());
            lcd.setCursor(0, 0);
            lcd.print(buffer);
            lcd.setCursor(0, 1);
            lcd.print("");
        } else {
            char first[17];
            sprintf(first, "%-16s", displayOutput.substring(0, indexOf).c_str());
            char second[17];
            sprintf(second, "%-16s", displayOutput.substring(indexOf + 1, displayOutput.length()).c_str());

            lcd.setCursor(0, 0);
            lcd.print(first);
            lcd.setCursor(0, 1);
            lcd.print(second);
        }
    }
}

void shiftOut595(BYTES_VAL_T value) {
    // Trigger a parallel Load to latch the state of the data lines
    digitalWrite(LATCH_OUT_PIN, LOW);

    // Loop to read each bit value from the serial out line of the SN74HC595N.
    for (int i = 0; i < DATA_WIDTH; i++) {
        // Set the corresponding bit in value.
        digitalWrite(DATA_OUT_PIN, !!(value & (1 << (DATA_WIDTH - i))));
        // Pulse the Clock (rising edge shifts the next bit).
        digitalWrite(CLOCK_OUT_PIN, HIGH);
        digitalWrite(CLOCK_OUT_PIN, LOW);
    }

    digitalWrite(LATCH_OUT_PIN, HIGH);
}

uint16_t OutputManager::map(uint16_t output) {
    uint16_t copy = 0;

    setLitMask(output, 0, 4, copy);
    setLitMask(output, 1, 5, copy);
    setLitMask(output, 2, 6, copy);
    setLitMask(output, 3, 7, copy);
    setLitMask(output, 4, 11, copy);
    setLitMask(output, 5, 12, copy);
    setLitMask(output, 6, 13, copy);
    setLitMask(output, 7, 14, copy);
    setLitMask(output, 8, 15, copy);

    return copy;
}

void OutputManager::setLitMask(const uint16_t output, const int bitPosition, const int mappedPosition, uint16_t &copy) {
    if (output & (1 << bitPosition)) {
        copy = copy | (1 << mappedPosition);
    }
}
