#include "pch.h"
#include "Interface.h"
#include "Tree.h"
#include <iostream>

Interface::Interface() {
	//constructer calls reset() because reset() is also used once broadcasting has finished.
	//this allows for continuous message broadcasts.
	this->reset();
}

Interface::~Interface() {
}

void Interface::reset() {
	this->broadcasting = true;
	this->role = SENDER; //set as sender initially, and change later if something is received.
	this->input_next_morse_char = true;
	this->encrypt_message = false;
	this->message_char.empty();
	this->mc_character = "";
	this->message = "";
}

void Interface::error() {
	std::cout << ":<" << std::endl;
}
void Interface::mc_setup_next_char(char user_input, Tree* tree) {
	if (this->mc_character.length() > 0) { //prevents empty characters being saved
		char human_character = ' ';
		if (encrypt_message) {
			std::string enc_morse_code = "";
			human_character = (*tree).find(tree, this->mc_character, &enc_morse_code); //returns letter for morse code character, i.e. .- returns A and updates the string pointer to contain an encrypted morse code
			for (char emc : enc_morse_code){
				this->send(&emc); //if encrypting, send after successful character input (rather than after each dot & dash)
			}
		}
		else {
			human_character = (*tree).find(tree, this->mc_character); //returns letter for morse code character, i.e. .- returns A
		}
		this->message.push_back(human_character); //store the character into the string
		this->mc_character = ""; //reset character string;
		this->input_next_morse_char = false; //reset the need for a new character
	}
}
void Interface::store_user_input(char user_input, Tree* tree) {

	std::cout << user_input << std::endl;

	if (!this->encrypt_message) {
		//if NOT encrypting, send instantly.
		//if you are encrypting, send during the save character process
		this->send(&user_input);
	}

	//start save procedure
	if (user_input == '.' || user_input == '-') {
		this->mc_character.push_back(user_input); //add input_morse_char onto the end of the vector at the end of the overall morse vector
	}
	else if (user_input == '#') { //character broadcast finished, start new character
		this->mc_setup_next_char(user_input, tree);
		if (this->encrypt_message)
			this->send(&user_input);
	}
	else if (user_input == '@') {
		this->mc_setup_next_char(user_input, tree); //allows end of message without sending end of character
		if (this->encrypt_message)
			this->send(&user_input);
		this->broadcasting = false;
	}
	else //invalid input
		this->error();
}
void Interface::print_message() {
	//loops through the message vector that's been built and outputs a character at a time

	// +++++++++++++  COMPLETE (non-encrypted) MESSAGE
	std::cout << "Before decryption, your entire message is: |" << this->message << "|" << std::endl;

	// +++++++++++++  COMPLETE (encrypted) MESSAGE
	std::cout << "After decryption, your entire message is: |";
	for (char m : this->message) {
		if (encrypt_message)
			this->decrypt(&m);
		char c = m;
		std::cout << m;
	}
	std::cout << "|\n" << std::endl;
}
void Interface::encrypt(char* c) {
	//operates a shift 2 policy
	//e.g. A becomes C, X becomes Z, 0 becomes 2, etc
	//Need to manually override due to ASCII numbering.
	//In ASCII, for example, 9 + 2 = ;
	if (*c == 'Y')
		(*c) = '0';
	else if (*c == 'Z')
		(*c) = '1';
	else if (*c == '8')
		(*c) = 'A';
	else if (*c == '9')
		(*c) = 'B';
	else
		*c = (*c) + 2;
}
void Interface::decrypt(char* c) {
	//Reverses encryption.
	//See notes in void Interface::encrypt(char* c) for details of manual override
	if (*c == 'A')
		(*c) = '8';
	else if (*c == 'B')
		(*c) = '9';
	else if (*c == '0')
		(*c) = 'Y';
	else if (*c == '1')
		(*c) = 'Z';
	else
		*c = (*c) - 2;
}
void Interface::send(char* c){
	if (*c == '.')
		std::cout << this->DOT + ((this->DOT / 100) * 2) << std::endl;
	else if (*c == '-')
		std::cout << this->DASH + ((this->DASH / 100) * 2) << std::endl;
	else if (*c == '#')
		std::cout << this->END_CHAR + ((this->END_CHAR / 100) * 2) << std::endl;
	else if (*c == '@')
		std::cout << this->END_MSG + ((this->END_MSG / 100) * 2) << std::endl;
	else
		this->error();

	std::cout << "50" << std::endl; //add some noise
}
void Interface::build_tree(Tree* tree) {
	//start inserting letters and corresponding morse code
	if (!(*tree).insert(tree, 'A', ".-", "-.-.")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'B', "-...", "-..")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'C', "-.-.", ".")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'D', "-..", "..-.")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'E', ".", "--.")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'F', "..-.", "....")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'G', "--.", "..")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'H', "....", ".---")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'I', "..")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'J', ".---")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'K', "-.-")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'L', ".-..")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'M', "--")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'N', "-.")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'O', "---")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'P', ".--.")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'Q', "--.-")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'R', ".-.")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'S', "...")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'T', "-")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'U', "..-")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'V', "...-")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'W', ".--")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'X', "-..-")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'Y', "-.--")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, 'Z', "--..")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, '0', "-----")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, '1', ".----")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, '2', "..---")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, '3', "...--")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, '4', "....-")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, '5', ".....")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, '6', "-....")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, '7', "--...")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, '8', "---..")) //if input is not valid, an error will be thrown
		this->error();
	if (!(*tree).insert(tree, '9', "----.")) //if input is not valid, an error will be thrown
		this->error();
}
void Interface::init() {
	Tree* tree = new Tree(); //create empty tree, ready to add morse code tree details

	while (1) {
		std::cout << "Go!" << std::endl;
		std::cout << "Do you want to encrypt? (Y=-.-- / N=-.). Use # to confirm and @ to start agin" << std::endl;
		// Y=-.-- / N=-.   Use # to confirm and @ to start agin"
		std::string enc_s = "";
		char enc_c = '~';
		while (enc_c != '#') {
			std::cin >> enc_c;
			if (enc_c == '@') //humans make mistakes. Allow the user to try again
				enc_s = "";
			else
				enc_s.push_back(enc_c);
		}
		if (enc_s == "-.--#")
			this->encrypt_message = true;
		else if(enc_s == "-.#")
			this->encrypt_message = false;

		build_tree(tree); //set up the morse code and character tree

		for (char m : enc_s) {
			//let the receiving device know whether we're using encryption or not.
			//sends the morse code for Y or N for Encrypt/Decrypt respectively
			//(also sticks a # on the end to signal end of character transmission)
			this->send(&m);
		}

		while (this->broadcasting) {
			char user_input = '~'; //set an invalid character as the default
			std::cout << "Enter a mose code CHARACTER (i.e. a dot or a dash, a # for end of character or an @ for end of message)" << std::endl;
			std::cin >> user_input;
			this->store_user_input(user_input, tree); //stores each morse code character (. or -), handles end-of-character and end-of_message codes, and encrypts as needed.
		}
		this->print_message();

		this->reset();
	}
}