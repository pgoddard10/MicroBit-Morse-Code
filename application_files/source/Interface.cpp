/*
 * File: Interface.cpp
 * Author: Paul Goddard
 * Date: 17-02-2019
 */

#include "MicroBit.h"
#include "Interface.h"
#include "Encrypter.h"

MicroBitPin P1(MICROBIT_ID_IO_P1, MICROBIT_PIN_P1, PIN_CAPABILITY_DIGITAL);
MicroBitButton buttonA(MICROBIT_PIN_BUTTON_A, MICROBIT_ID_BUTTON_A);
MicroBitImage dot("0,0,0,0,0\n0,255,255,255,0\n0,255,255,255,0\n0,255,255,255,0\n0,0,0,0,0\n");
MicroBitImage dash("0,0,0,0,0\n0,0,0,0,0\n0,255,255,255,0\n0,0,0,0,0\n0,0,0,0,0\n");

Interface::Interface() {
	this->broadcasting = true;
	this->new_input_morse_char = true;
	this->error = true;
	this->role = SENDER; //set as sender initially, and change later if something is received.
	this->pressed = false;

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

void Interface::init(MicroBit* uBit) {
    
    (*uBit).display.scrollAsync("Go");
	Encrypter enc = Encrypter();
    
    while(this->broadcasting) {
		
		if (this->new_input_morse_char) { //create a new row in the morse vector to store the character
			this->message.push_back(std::vector<char>());
			this->new_input_morse_char = false;
		}
		
		// read current number of milliseconds 
		uint64_t reading = system_timer_current_time();
		// loop while button A pressed
		if(this->role==SENDER) { //if role is to send
			while (buttonA.isPressed())
				pressed = true;
		}
		while(P1.getDigitalValue()) {
			pressed = true;
			if(this->role==SENDER) //if I've received something, stop be being a sender!
				this->role = RECEIVER;
		}
		// time of loop execution
		uint64_t delta = system_timer_current_time() - reading;
		
		char input_morse_char = ' ';
        if (pressed) { // if button was pressed
            // geater than 5 secs
            if (delta > 5000) { // > 1 sec
				input_morse_char = '@';
                (*uBit).display.print("@");
            }
            // geater than 2 seconds but less than 5 secs
            else if (delta > 2000) { // > 2 secs
				input_morse_char = '\\';
                (*uBit).display.print("\\");
            }
            // geater than 1 second but less than 2 secs
            else if (delta > 1000) { // > 1 sec
				input_morse_char = '-';
                (*uBit).display.print(dash);
            }
            // greater than 1/2 sec, but less than 1 sec
            else if (delta > 500) { // > 0.5 sec
				input_morse_char = '.';
                (*uBit).display.print(dot);
            }
            (*uBit).sleep(500); //display the morse_char for 0.5 seconds
            
			if (input_morse_char == '.' || input_morse_char == '-')
				this->message[this->message.size() - 1].push_back(input_morse_char); //add input_morse_char onto the end of the vector at the end of the overall morse vector
			else if (input_morse_char == '\\') { //single backslash but \ is escape character, therefore need to have two slashes
				this->new_input_morse_char = true;

				char hc = this->get_human_char(&this->message[this->message.size() - 1]);
				std::string morse = enc.swap_human_to_morse_code(&this->morse_to_human_char_map, enc.encrypt_human_character(hc));
				
				if(this->role==SENDER)
					send(uBit,morse);
				//(*uBit).display.print(this->get_human_char(&this->message[this->message.size() - 1]));
				(*uBit).display.print(enc.decrypt_human_character(this->get_human_char(&this->message[this->message.size() - 1])));
                (*uBit).sleep(500);
			}
			else if (input_morse_char == '@')
				this->broadcasting = false;
			else
				this->error = true;

			if ((!this->error) && (this->role == SENDER)) {
				//send(new_input_morse_char);
			}

            this->pressed = false;
            (*uBit).display.clear();
        }
	} //sender has indicated that the entire message has finished
	(*uBit).display.scroll(this->get_human_message()); //print entire message
}
char Interface::get_human_char(std::vector<char>* message_n) {
	//converts series of dots and dashes into the human-readable character (e.g. a . (dot) is the letter E
	std::string morse_letter = "";
	for (int i = 0; i < (*message_n).size(); i++) {
		char c = (*message_n).at(i);
		morse_letter.push_back(c);
	}
	return this->morse_to_human_char_map[morse_letter];
}
ManagedString Interface::get_human_message() {
	//converts the entire broadcasted message into a string, which should make a word. e.g. A . (dot) +  .- (dot-dash) returns EA
	ManagedString human_letter = "";
	for (int i = 0; i < this->message.size(); i++) {
		char c = this->get_human_char(&message[i]);
		human_letter = human_letter + c;
	}
	return human_letter;
}
void Interface::send(MicroBit* uBit, std::string morse) {
	//physically send a value from pin 1
	for(char& c : morse) {
		uint64_t time = 10;
		if(c=='.')
			time = 510;
		else if(c=='-')
			time = 1010;
		else if(c=='\\')
			time = 2020;
		else if(c=='@')
			time = 5010;
		
		P1.setDigitalValue(1);
		(*uBit).sleep(time);
		P1.setDigitalValue(0);
	}
}
