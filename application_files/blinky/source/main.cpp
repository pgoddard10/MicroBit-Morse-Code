/**
 * File: main.cpp
 * Author: Benedict R. Gaster
 * Date: 21-01-2018
 * Desc: Simple LED blinky example (pin 1) for micro:bit
 * Copyright: University of West of England 2017
 */
#include "MicroBit.h"

MicroBit uBit;

MicroBitPin P1(MICROBIT_ID_IO_P1, MICROBIT_PIN_P1, PIN_CAPABILITY_DIGITAL);

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    // we use asunc here, to allow the led to start flashing straight away,
    // i.e. not have to wait for message to be displayed
    uBit.display.scrollAsync("Blinky");

    while(1) {
        // turn LED on
        P1.setDigitalValue(1);
        uBit.sleep(500);
        // turn LED off
        P1.setDigitalValue(0);
        uBit.sleep(500);
    }

    release_fiber();
}
