/*
 * File: Encrypter.cpp
 * Author: Paul Goddard
 * Date: 17-02-2019
 */

#include "MicroBit.h"
#include "Encrypter.h"
#include "Interface.h"
#include <string>
#include <map>

Encrypter::Encrypter() {
}


Encrypter::~Encrypter() {
}

std::string Encrypter::swap_human_to_morse_code(std::map<std::string, char>* morse_to_human_char_map, char letter) {
	for (auto it = (*morse_to_human_char_map).begin(); it != (*morse_to_human_char_map).end(); ++it) {
		if (it->second == letter)
			return it->first;
	}
}
char Encrypter::encrypt_human_character(char letter) {
	if (letter == '8')
		return 'A';
	else if (letter == '9')
		return 'B';
	else if (letter == 'Y')
		return '0';
	else if (letter == 'Z')
		return '1';
	else
		return char(letter + 2);
}
char Encrypter::decrypt_human_character(char letter) {
	if (letter == 'A')
		return '8';
	else if (letter == 'B')
		return '9';
	else if (letter == '0')
		return 'Y';
	else if (letter == '1')
		return 'Z';
	else
		return char(letter - 2);
}
