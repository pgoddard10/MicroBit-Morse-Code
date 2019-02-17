/**
 * File: main.cpp
 * Author: Benedict R. Gaster
 * Date: 11-02-2018
 * Desc: Example showing how to test length of button press
 * Copyright: University of West of England 2017
 */
#include "MicroBit.h"

MicroBit uBit;


MicroBitButton buttonA(MICROBIT_PIN_BUTTON_A, MICROBIT_ID_BUTTON_A);

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    bool pressed = false;

    while(1) {
        // read current number of milliseconds
        uint64_t reading = system_timer_current_time();

        // loop while button A pressed
        while (buttonA.isPressed()) {
            pressed = true;
        }

        // time of loop execution
        uint64_t delta = system_timer_current_time() - reading;

        // if button was pressed
        if (pressed) {
            // geater than a second
            if (delta > 1000) { // > 1 sec
                uBit.display.print("O");
                uBit.sleep(500);
            }
            // greater than 1/2 sec, but less than sec
            else if (delta > 500) {
                uBit.display.print("U");
                uBit.sleep(500);
            }

            pressed = false;
            uBit.display.clear();
        }
    }

    release_fiber();
}
