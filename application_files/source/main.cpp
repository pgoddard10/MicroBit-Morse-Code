/*
 * File: main.cpp
 * Author: Paul Goddard
 * Date: 17-02-2019
 */

#include "MicroBit.h"
#include "Interface.h"
#include "Encrypter.h"

MicroBit uBit;

int main() {
    /* init uBit and seed pseudo random number generator */
    uBit.init();
    
    Interface mc = Interface();
	mc.init(&uBit);

    release_fiber();
}
