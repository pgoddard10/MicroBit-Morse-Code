#include "MicroBit.h"
#include "Interface.h"
#include "Tree.h"
#include <string>

MicroBit uBit;
MicroBitPin P1(MICROBIT_ID_IO_P1, MICROBIT_PIN_P1, PIN_CAPABILITY_DIGITAL);
MicroBitButton buttonA(MICROBIT_PIN_BUTTON_A, MICROBIT_ID_BUTTON_A);

Interface::Interface() {
	uBit.init();
	
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
	this->pressed = false;
}

void Interface::error() {
	uBit.display.scroll("*.*");
	uBit.sleep(1000);
}
void Interface::mc_setup_next_char(char user_input, Tree* tree) {
	if (this->mc_character.length() > 0) { //prevents empty characters being saved
		char human_character = ' ';
		if (encrypt_message) {
			std::string enc_morse_code = "";
			human_character = (*tree).find(tree, this->mc_character, &enc_morse_code); //returns letter for morse code character, i.e. .- returns A and updates the string pointer to contain an encrypted morse code
			if(this->role==SENDER){
				for (char emc : enc_morse_code)
					this->send(&emc); //if encrypting, send after successful character input (rather than after each dot & dash)
			}
		}
		else {
			human_character = (*tree).find(tree, this->mc_character); //returns letter for morse code character, i.e. .- returns A
		}
		uBit.display.print(human_character);
		this->message.push_back(human_character); //store the character into the string
		this->mc_character = ""; //reset character string;
		this->input_next_morse_char = false; //reset the need for a new character
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
	if (user_input == '.' || user_input == '-') {
		this->mc_character = this->mc_character + user_input; //add input_morse_char onto the end of the vector at the end of the overall morse vector
	}
	else if (user_input == '#') { //character broadcast finished, start new character
		this->mc_setup_next_char(user_input, tree);
		if(this->role==SENDER) {
			if (this->encrypt_message)
				this->send(&user_input);
		}
	}
	else if (user_input == '@') {
		this->mc_setup_next_char(user_input, tree); //allows end of message without sending end of character
		if(this->role==SENDER) {
			if (this->encrypt_message)
				this->send(&user_input);
		}
		this->broadcasting = false;
	}
	else //invalid input
		this->error();
}
void Interface::print_message() {
	//loops through the message vector that's been built and outputs a character at a time

	// +++++++++++++  COMPLETE (non-encrypted) MESSAGE
	uBit.display.scroll("enc: ");
	for (char m : this->message) {
		char c = m;
		uBit.display.print(m);
		uBit.sleep(500);
	}
	uBit.display.clear();
	uBit.sleep(1000);

	// +++++++++++++  COMPLETE (encrypted) MESSAGE
	uBit.display.scroll("Your message: ");
	for (char m : this->message) {
		if (encrypt_message)
			this->decrypt(&m);
		char c = m;
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
	uint64_t time = 0;
	if (*c == '.')
		time = this->DOT + 25;
	else if (*c == '-')
		time = this->DASH + 50;
	else if (*c == '#')
		time = this->END_CHAR + 50;
	else if (*c == '@')
		time = this->END_MSG + 50;
	else
		this->error();
	
	uBit.display.scroll("send");
	uBit.display.print(*c);
	
	P1.setDigitalValue(1);
	uBit.sleep(time);
	P1.setDigitalValue(0);
	
	//noise
	P1.setDigitalValue(0);
	uBit.sleep(30);
	P1.setDigitalValue(0);

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
		//~ uBit.display.scroll("Encrypt?");
		//~ // Y=-.-- / N=-.   Use # to confirm and @ to start agin"
		//~ std::string enc_s = "";
		//~ char enc_c = '~';
		//~ while ((enc_c != '#') && (enc_s!="-.--" || enc_s!="-.")) {
			//~ // read current number of milliseconds 
			//~ uint64_t reading = system_timer_current_time();
			//~ // loop while button A pressed
			//~ if(this->role==SENDER) { //if role is to send
				//~ while (buttonA.isPressed())
					//~ this->pressed = true;
			//~ }
			//~ while(P1.getDigitalValue()) {
				//~ this->pressed = true;
				//~ if(this->role==SENDER) {//if I've received something, stop be being a sender!
					//~ this->role = RECEIVER;
				//~ }
			//~ }
			//~ // time of loop execution
			//~ uint64_t delta = system_timer_current_time() - reading;
			
			//~ if(this->pressed){
				//~ //ultimately switches on/off encryption
				//~ //let the receiving device know whether we're using encryption or not.
				//~ //sends the morse code for Y or N for Encrypt/Decrypt respectively
				
				//~ if(delta > this->END_MSG){
					//~ enc_c = '@';
					//~ enc_s = "";
					//~ uBit.display.print(enc_c);
					//~ if(this->role==SENDER)
						//~ this->send(&enc_c);
				//~ }
				//~ else if(delta > this->END_CHAR){
					//~ enc_c = '#';
					//~ enc_s.push_back('#');
					//~ uBit.display.print(enc_c);
					//~ if(this->role==SENDER)
						//~ this->send(&enc_c);
				//~ }
				//~ else if(delta > this->DASH){
					//~ enc_c = '-';
					//~ enc_s.push_back('-');
					//~ uBit.display.print(enc_c);
					//~ if(this->role==SENDER)
						//~ this->send(&enc_c);
				//~ }
				//~ else if(delta > this->DOT){
					//~ enc_c = '.';
					//~ enc_s.push_back('.');
					//~ uBit.display.print(enc_c);
					//~ if(this->role==SENDER)
						//~ this->send(&enc_c);
				//~ }
				//~ else {
					//~ //noise / nothing happening
				//~ }
				//~ uBit.sleep(500);
				//~ uBit.display.clear();
			//~ }
			//~ this->pressed = false;
		//~ }
		//~ if (enc_s == "-.--#")
			//~ this->encrypt_message = true;
		//~ else if(enc_s == "-.#")
			//~ this->encrypt_message = false;
			
		this->encrypt_message = true; //OVERRIDE!!!!!!!
			
		uBit.display.scroll(this->encrypt_message);


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

		this->reset();
	}
	
	
	release_fiber();
}
