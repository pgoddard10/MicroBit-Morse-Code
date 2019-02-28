/*
 * File: main.cpp
 * Author: Paul Goddard
 * Date: 20-02-2019
 */

#include "MicroBit.h"
#include "Interface.h"
#include "Tree.h"
#include <string>

MicroBit uBit;
MicroBitPin P1(MICROBIT_ID_IO_P1, MICROBIT_PIN_P1, PIN_CAPABILITY_DIGITAL);
MicroBitButton buttonA(MICROBIT_PIN_BUTTON_A, MICROBIT_ID_BUTTON_A);

Interface::Interface() {
	uBit.init(); //init the MicroBit object to allow use of the specific device functions at runtime
}

Interface::~Interface() {
}

void Interface::init() {
	//set the default values for use within this application.
	//called in main and after each successfully broadcasted message (to allow a further message to be sent)
	
	this->broadcasting = true;
	this->role = SENDER; //set as sender initially, and change later if something is received.
	this->encrypt_message = false;
	this->mc_character = "";
	this->message = "";
	this->pressed = false;
}

void Interface::error(ManagedString err_msg) {
	//display error message and pause.
	uBit.display.scrollAsync(err_msg);
	uBit.sleep(1000);
}
void Interface::mc_setup_next_char(char user_input, Tree* tree) {
	// Handles the end of a morse code string, e.g. .- is the letter A.
	// The info about each individual letter needs storing and, if applicable, sending
	
	if (this->mc_character.length() > 0) { //prevents empty characters being saved
		char human_character = ' '; //temp variable
		if (encrypt_message) { //if encryption is on
			std::string enc_morse_code = ""; //temp variable
			human_character = (*tree).find(tree, this->mc_character, &enc_morse_code); //returns letter for morse code character, i.e. .- returns A and updates the string pointer to contain an encrypted morse code
			if(this->role==SENDER){ //if this device is responsible for sending, send the entire character's morse code
				uBit.display.scroll("Sending: ");
				for (char emc : enc_morse_code) //send one dot/dash at a time
					this->send(&emc); //if encrypting, send after successful character input (rather than after each dot & dash)
			}
		}
		else {
			human_character = (*tree).find(tree, this->mc_character); //returns letter for morse code character, i.e. .- returns A
		}
		uBit.display.print(human_character);
		this->message.push_back(human_character); //store the character into the string
		this->mc_character = ""; //reset character string;
	}
}
void Interface::store_user_input(char user_input, Tree* tree) {	
	
	if (!this->encrypt_message) {
		//if NOT encrypting, send instantly.
		//if you are encrypting, send during the save character process
		if(this->role==SENDER) {
			this->send(&user_input);
		}
	}

	//start save procedure
	if (user_input == '.' || user_input == '-') { //verify that a dot or  dash has been input
		this->mc_character = this->mc_character + user_input; //add input_morse_char onto the end of the vector at the end of the overall morse vector
	}
	else if (user_input == '#') { //character broadcast finished, start new character
		this->mc_setup_next_char(user_input, tree);
		if(this->role==SENDER && this->encrypt_message)
			this->send(&user_input); //send end of character (i.e. a hash)
	}
	else if (user_input == '@') {
		this->mc_setup_next_char(user_input, tree); //allows end of message without sending end of character
		if(this->role==SENDER && this->encrypt_message)
			this->send(&user_input); //send end of message (i.e. a @)
		this->broadcasting = false; //reset broadcasting to false as the message is over
	}
	else //invalid input character (should be impossible)
		this->error("No save. Invalid input");
}
void Interface::print_message() {
	//loops through the message vector that's been built and prints a character at a time

	// COMPLETE (encrypted) MESSAGE
	if((this->role==RECEIVER)&&(this->encrypt_message)){
		uBit.display.scroll("Received: ");
		//print the encrypted message
		for (char m : this->message) { //loop through each char
			uBit.display.print(m); //print char
			uBit.sleep(500);
		}
		uBit.display.clear();
		uBit.sleep(1000);
		uBit.display.scroll("Decrypted: ");
		//continue below, which is the unencrypted message
	}
	else {
		uBit.display.scroll("Message: ");
	}
	
	// COMPLETE (unencrypted) MESSAGE
	for (char m : this->message) {
		if ((this->encrypt_message) && (this->role==RECEIVER)) //message is encrypted during the sending process, so only decrypt if receiving
			this->decrypt(&m);
		uBit.display.print(m);
		uBit.sleep(500);
	}
	uBit.display.clear();
	uBit.sleep(1000);
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
	//sends individual character
	//converts each character (. - # @) into a period of time that's defined in the header file
	//sends a high value (1) for the period of time specificied.
	//to ensure that the receiving device can tell the difference when send is called multiple times, we sleep for 1 second between sends
	
	uint64_t time = 0;
	if (*c == '.')
		time = this->DOT;
	else if (*c == '-')
		time = this->DASH;
	else if (*c == '#')
		time = this->END_CHAR;
	else if (*c == '@')
		time = this->END_MSG;
	else
		this->error("No send. Invalid char.");
	
	uBit.display.print(*c); //show the character that's being sent (. - # @)
	
	P1.setDigitalValue(1);
	uBit.sleep(time);
	P1.setDigitalValue(0);
	
	//send some noise
	uBit.sleep(500);
	uBit.display.clear(); //clear the screen, helping the user to see the gap between two of the same character
	uBit.sleep(500);

}
void Interface::build_tree(Tree* tree) {
	//start inserting letters and corresponding morse code into the tree
	
	// insert a new tree node containing this human-readable letter, the original morse-code and the encrypted morse-code strings
	int err = (*tree).insert(tree, 'A', ".-", "-.-.");
	err |=  (*tree).insert(tree, 'B', "-...", "-..");
	err |=  (*tree).insert(tree, 'C', "-.-.", ".");
	err |=  (*tree).insert(tree, 'D', "-..", "..-.");
	err |=  (*tree).insert(tree, 'E', ".", "--.");
	err |=  (*tree).insert(tree, 'F', "..-.", "....");
	err |=  (*tree).insert(tree, 'G', "--.", "..");
	err |=  (*tree).insert(tree, 'H', "....", ".---");
	err |=  (*tree).insert(tree, 'I', "..", "-.-");
	err |=  (*tree).insert(tree, 'J', ".---", ".-..");
	err |=  (*tree).insert(tree, 'K', "-.-", "--");
	err |=  (*tree).insert(tree, 'L', ".-..", "-.");
	err |=  (*tree).insert(tree, 'M', "--", "---");
	err |=  (*tree).insert(tree, 'N', "-.", ".--.");
	err |=  (*tree).insert(tree, 'O', "---", "--.-");
	err |=  (*tree).insert(tree, 'P', ".--.", ".-.");
	err |=  (*tree).insert(tree, 'Q', "--.-", "...");
	err |=  (*tree).insert(tree, 'R', ".-.", "-");
	err |=  (*tree).insert(tree, 'S', "...", "..-");
	err |=  (*tree).insert(tree, 'T', "-", "...-");
	err |=  (*tree).insert(tree, 'U', "..-", ".--");
	err |=  (*tree).insert(tree, 'V', "...-", "-..-");
	err |=  (*tree).insert(tree, 'W', ".--", "-.--");
	err |=  (*tree).insert(tree, 'X', "-..-", "--..");
	err |=  (*tree).insert(tree, 'Y', "-.--", "-----");
	err |=  (*tree).insert(tree, 'Z', "--..", ".----");
	err |=  (*tree).insert(tree, '0', "-----", "..---");
	err |=  (*tree).insert(tree, '1', ".----", "...--");
	err |=  (*tree).insert(tree, '2', "..---", "....-");
	err |=  (*tree).insert(tree, '3', "...--", ".....");
	err |=  (*tree).insert(tree, '4', "....-", "-....");
	err |=  (*tree).insert(tree, '5', ".....", "--...");
	err |=  (*tree).insert(tree, '6', "-....", "---..");
	err |=  (*tree).insert(tree, '7', "--...", "----.");
	err |=  (*tree).insert(tree, '8', "---..", ".-");
	err |=  (*tree).insert(tree, '9', "----.", "-...");
	
	if(err > 0) this->error("Error inserting to tree."); //if any of the above inserts failed, call the error function
}
void Interface::run() {
	// The main function that is responsible for interaction with the user and controlling the flow of events
	
	Tree* tree = new Tree(); //create empty tree, ready to add morse code tree details

	while (1) {
		uBit.display.scroll("Encrypt?");
		// Y=-.-- / N=-.   Use # to confirm and @ to start agin"
		std::string enc_s = "";
		char enc_c = '~';
		bool valid_str = false;
		while (!valid_str) {
			// read current number of milliseconds 
			uint64_t reading = system_timer_current_time();
			// loop while button A pressed
			if(this->role==SENDER) { //if role is to send
				while (buttonA.isPressed())
					this->pressed = true;
			}
			while(P1.getDigitalValue()) {
				this->pressed = true;
				if(this->role==SENDER) { //if I've received something, stop be being a sender!
					this->role = RECEIVER;
				}
			}
			// time of loop execution
			uint64_t delta = system_timer_current_time() - reading;
			
			if(this->pressed){
				//ultimately switches on/off encryption
				//let the receiving device know whether we're using encryption or not.
				//sends the morse code for Y or N for Encrypt/Decrypt respectively
				
				if(delta > this->END_MSG){
					enc_c = '@';
					enc_s = "";
					uBit.display.print(enc_c);
					if(this->role==SENDER)
						this->send(&enc_c);
				}
				else if(delta > this->END_CHAR){
					enc_c = '#';
					enc_s.push_back('#');
					uBit.display.print(enc_c);
					if(this->role==SENDER)
						this->send(&enc_c);
				}
				else if(delta > this->DASH){
					enc_c = '-';
					enc_s.push_back('-');
					uBit.display.print(enc_c);
					if(this->role==SENDER)
						this->send(&enc_c);
				}
				else if(delta > this->DOT){
					enc_c = '.';
					enc_s.push_back('.');
					uBit.display.print(enc_c);
					if(this->role==SENDER)
						this->send(&enc_c);
				}
				else {
					//noise / nothing happening
				}
				uBit.sleep(500);
				uBit.display.clear();
			}
			this->pressed = false;
			
			if (enc_s == "--#") { //yes
				valid_str = true;
				this->encrypt_message = true; //turn encryption on
			}
			else if(enc_s == "..#") { //no
				valid_str = true;
				this->encrypt_message = false; //turn encryption off
			}
			else { //invalid input
				if(enc_c == '#') {
					this->error("Invalid");
					enc_s = ""; //reset to empty
					enc_c = '~'; //reset to invalid char
				}
			}
		}
			
		if(this->encrypt_message) //confirm back to the user whether encryption is on or off.
			uBit.display.scroll("Encrypt ON");
		else
			uBit.display.scroll("Encrypt OFF");

		build_tree(tree); //set up the morse code and character tree

		if(this->role==SENDER)
			uBit.display.scroll("Enter Code:");
		else
			uBit.display.scroll("Wait..");
		while (this->broadcasting) {
			char user_input = '~'; //set an invalid character as the default			
			
			// read current number of milliseconds 
			uint64_t reading = system_timer_current_time();
			// loop while button A pressed
			if(this->role==SENDER) { //if role is to send
				while (buttonA.isPressed())
					this->pressed = true;
			}
			while(P1.getDigitalValue()) {
				this->pressed = true;
				if(this->role==SENDER) //if I've received something, stop be being a sender!
					this->role = RECEIVER;
			}
			// time of loop execution
			uint64_t delta = system_timer_current_time() - reading;
			
			if(this->pressed){
				if(delta > this->END_MSG){
					user_input = '@';
					this->store_user_input(user_input, tree); //stores each morse code character (. or -), handles end-of-character and end-of_message codes, and encrypts as needed.
					uBit.display.print(user_input);
				}
				else if(delta > this->END_CHAR){
					user_input = '#';
					this->store_user_input(user_input, tree); //stores each morse code character (. or -), handles end-of-character and end-of_message codes, and encrypts as needed.
					uBit.display.print(user_input);
				}
				else if(delta > this->DASH){
					user_input = '-';
					this->store_user_input(user_input, tree); //stores each morse code character (. or -), handles end-of-character and end-of_message codes, and encrypts as needed.
					uBit.display.print(user_input);
				}
				else if(delta > this->DOT){
					user_input = '.';
					this->store_user_input(user_input, tree); //stores each morse code character (. or -), handles end-of-character and end-of_message codes, and encrypts as needed.
					uBit.display.print(user_input);
				}
				else {
					//noise / nothing happening
				}
				uBit.sleep(500);
				uBit.display.clear();
			}
			this->pressed = false;
			
		}
		this->print_message();
		uBit.sleep(500);

		this->init(); //reset the variables back to default and allow the user to send another message
	}
	
	
	release_fiber();
}
