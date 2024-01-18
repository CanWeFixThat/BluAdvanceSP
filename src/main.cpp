#include <Arduino.h>
#include <EEPROM.h>

const byte BUTTON_PIN = 3; // Pin number for the button
const int BT_PAIR_PIN = 0; // Pin for initiating Bluetooth pairing
const int BT_POWER_PIN = 4; // Pin to control Bluetooth power

// Time intervals for different button click types
const unsigned long DOUBLE_CLICK_INTERVAL = 500; // Time window for detecting a double click (in milliseconds)
const unsigned long LONG_PRESS_INTERVAL = 500; // Time for considering a button press as a long press (in milliseconds)
const unsigned long DEBOUNCE_INTERVAL = 50; // Time for button debounce (in milliseconds)

const int EEPROM_BT_POWER_ADDR = 0; // EEPROM address to store the state of BT_POWER_PIN

unsigned long lastButtonChangeTime = 0; // Tracks the last time the button state changed
unsigned long firstClickTime = 0; // Timestamp of the first click in a series
unsigned long lastClickTime = 0; // Timestamp of the last click

int clickCount = 0; // Counter for button clicks

bool lastButtonState = HIGH; // Stores the last state of the button
bool buttonPressed = false; // Indicates if the button is currently pressed
bool longPressActive = false; // Flag to indicate if a long press has occurred
bool doubleClickFlag = false; // Flag for double click action

void triggerPairing() {
    // Function to trigger Bluetooth pairing
    digitalWrite(BT_PAIR_PIN, HIGH);
    delay(300);
    digitalWrite(BT_PAIR_PIN, LOW);
}

void toggleBluetoothPower() {
    // Function to toggle Bluetooth power state
    bool currentState = digitalRead(BT_POWER_PIN);
    bool newState = !currentState;
    digitalWrite(BT_POWER_PIN, newState);

    // Update the new state in EEPROM
    EEPROM.update(EEPROM_BT_POWER_ADDR, newState);
}

void setup() {
    // Initial setup for pins and EEPROM
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(BT_PAIR_PIN, OUTPUT);
    pinMode(BT_POWER_PIN, OUTPUT);

    // Read saved Bluetooth power state from EEPROM and apply
    bool savedState = EEPROM.read(EEPROM_BT_POWER_ADDR);
    digitalWrite(BT_POWER_PIN, savedState);
}

void updateButtonState() {
    // Function to handle button state changes
    bool currentButtonState = digitalRead(BUTTON_PIN);
    unsigned long currentTime = millis();

    // Debouncing the button
    if (currentTime - lastButtonChangeTime > DEBOUNCE_INTERVAL) {
        if (currentButtonState != lastButtonState) {
            lastButtonChangeTime = currentTime;
            lastButtonState = currentButtonState;

            if (currentButtonState == LOW) {
                // Button is pressed
                buttonPressed = true;
                longPressActive = false;
                clickCount++;
                if (clickCount == 1) {
                    firstClickTime = currentTime;
                }
                lastClickTime = currentTime;
            } else {
                // Button is released
                buttonPressed = false;
            }
        }
    }

    // Detecting double click
    if (currentTime - lastClickTime > DOUBLE_CLICK_INTERVAL && clickCount > 0) {
        if (clickCount == 2 && lastClickTime - firstClickTime <= DOUBLE_CLICK_INTERVAL) {
            triggerPairing();
        }
        clickCount = 0; // Reset click counter
    }

    // Detecting long press
    if (buttonPressed && currentTime - lastButtonChangeTime > LONG_PRESS_INTERVAL && !longPressActive) {
        toggleBluetoothPower();
        longPressActive = true;
    }
}

void loop() {
    // Main loop to continuously check button state
    updateButtonState();
}
