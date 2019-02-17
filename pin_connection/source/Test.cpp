#include "Test.h"

Test::Test() {
	this->hi = 1;
}


Test::~Test() {
}

void Test::my_uBit_test(MicroBit* bitty) {
	(*bitty).display.scroll("bitty");
	(*bitty).display.scroll(this->hi);
}
