// Morse_Testing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Interface.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>

std::string swap_human_to_morse_code(std::map<std::string, char>* morse_to_human_char_map, char letter) {
	for (auto it = (*morse_to_human_char_map).begin(); it != (*morse_to_human_char_map).end(); ++it) {
		if (it->second == letter)
			return it->first;
	}
}
char encrypt_human_character(char letter) {
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
char decrypt_human_character(char letter) {
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

int main()
{
	//Interface mc = Interface();
	//mc.init();

	std::map<std::string, char> morse_to_human_char_map = {
		{".-",		'A'},
		{"-...",	'B'},
		{"-.-.",	'C'},
		{"-..",		'D'},
		{".",		'E'},
		{"..-.",	'F'},
		{"--.",		'G'},
		{"....",	'H'},
		{"..",		'I'},
		{".---",	'J'},
		{"-.-",		'K'},
		{".-..",	'L'},
		{"--",		'M'},
		{"-.",		'N'},
		{"---",		'O'},
		{".--.",	'P'},
		{"--.-",	'Q'},
		{".-.",		'R'},
		{"...",		'S'},
		{"-",		'T'},
		{"..-",		'U'},
		{"...-",	'V'},
		{".--",		'W'},
		{"-..-",	'X'},
		{"-.--",	'Y'},
		{"--..",	'Z'},
		{"-----",	'0'},
		{".----",	'1'},
		{"..---",	'2'},
		{"...--",	'3'},
		{"....-",	'4'},
		{".....",	'5'},
		{"-....",	'6'},
		{"--...",	'7'},
		{"---..",	'8'},
		{"----.",	'9'}
	};


	std::cout << swap_human_to_morse_code(&morse_to_human_char_map, encrypt_human_character('C')) << std::endl;
	std::cout << morse_to_human_char_map[swap_human_to_morse_code(&morse_to_human_char_map, encrypt_human_character('C'))] << std::endl;
	std::cout << decrypt_human_character(morse_to_human_char_map[swap_human_to_morse_code(&morse_to_human_char_map, encrypt_human_character('C'))]) << std::endl;
	std::cout << "\n\n";

	std::cout << swap_human_to_morse_code(&morse_to_human_char_map, encrypt_human_character('A')) << std::endl;
	std::cout << swap_human_to_morse_code(&morse_to_human_char_map, encrypt_human_character('B')) << std::endl;
	std::cout << swap_human_to_morse_code(&morse_to_human_char_map, encrypt_human_character('C')) << std::endl;
	std::cout << swap_human_to_morse_code(&morse_to_human_char_map, encrypt_human_character('D')) << std::endl;
	std::cout << encrypt_human_character('B');
	std::cout << encrypt_human_character('C');
	std::cout << encrypt_human_character('D');
	std::cout << encrypt_human_character('E');
	std::cout << encrypt_human_character('F');
	std::cout << "...";
	std::cout << encrypt_human_character('U');
	std::cout << encrypt_human_character('V');
	std::cout << encrypt_human_character('W');
	std::cout << encrypt_human_character('X');
	std::cout << encrypt_human_character('Y');
	std::cout << encrypt_human_character('Z');
	std::cout << "...";
	std::cout << encrypt_human_character('0');
	std::cout << encrypt_human_character('1');
	std::cout << encrypt_human_character('2');
	std::cout << "...";
	std::cout << encrypt_human_character('7');
	std::cout << encrypt_human_character('8');
	std::cout << encrypt_human_character('9');
	std::cout << "...";

#if 0
	//		SANDBOX

#endif
}