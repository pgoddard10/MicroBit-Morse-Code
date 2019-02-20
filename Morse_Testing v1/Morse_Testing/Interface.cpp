#include "pch.h"
#include "Interface.h"
#include <iostream>

Interface::Interface() {
	this->broadcasting = true;
	this->new_input_morse_char = true;
	this->error = true;
	this->role = SENDER;

	morse_to_human_char_map = {
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
}


Interface::~Interface() {
}

void Interface::init() {
	while (broadcasting) {
		build_message();
	}
}

void Interface::build_message() {
	if (this->new_input_morse_char) { //create a new row in the morse vector to store the character
		this->message.push_back(std::vector<char>());
		this->new_input_morse_char = false;
	}

	char input_morse_char = ' ';
	if (this->role == SENDER) {
		std::cout << "Enter a morse code character: .	-	\\	/" << std::endl;
		std::cin >> input_morse_char;
		//input_morse_char = onPress(getLengthOfPress);
	}
	else if (this->role == RECEIVER) {
		//input_morse_char = onData(getData);
	}

	if (input_morse_char == '.' || input_morse_char == '-')
		this->message[this->message.size() - 1].push_back(input_morse_char); //add input_morse_char onto the end of the vector at the end of the overall morse vector
	else if (input_morse_char == '\\') { //single backslash but \ is escape character, therefore need to have two slashes
		this->new_input_morse_char = true;
		this->get_human_char(&this->message[this->message.size() - 1]);
	}
	else if (input_morse_char == '/')
		this->broadcasting = false;
	else
		this->error = true;

	if ((!this->error) && (this->role == SENDER)) {
		//send(new_input_morse_char);
	}
}
char Interface::get_human_char(std::vector<char>* message_n) {
	//converts series of dots and dashes into the human-readable character
	std::string morse_letter = "";
	for (int i = 0; i < (*message_n).size(); i++) {
		char c = (*message_n).at(i);
		morse_letter.push_back(c);
	}
	return this->morse_to_human_char_map[morse_letter];
}