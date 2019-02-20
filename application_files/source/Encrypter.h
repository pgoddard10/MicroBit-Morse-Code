/*
 * File: Encrypter.h
 * Author: Paul Goddard
 * Date: 17-02-2019
 */

#ifndef ENCRYPTER_H
#define ENCRYPTER_H

#include "MicroBit.h"
#include <string>
#include <map>

class Encrypter
{
public:
	Encrypter();
	~Encrypter();
	std::string swap_human_to_morse_code(std::map<std::string, char>* morse_to_human_char_map, char letter);
	char encrypt_human_character(char letter);
	char decrypt_human_character(char letter);
};
#endif
