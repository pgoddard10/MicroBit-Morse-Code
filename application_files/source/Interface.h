#ifndef INTERFACE_H
#define INTERFACE_H

#include "Tree.h"
#include "MicroBit.h"
#include <string>
#include <vector>

class Interface
{
public:
	Interface();
	~Interface();
	void run();
	void init();
	void error(ManagedString err);
	void store_user_input(char user_input, Tree* tree);
	void mc_setup_next_char(char user_input, Tree* tree);
	void encrypt(char* c);
	void decrypt(char* c);
	void print_message();
	void send(char* c);
	void build_tree(Tree* tree);

private:
	enum Role { //whether the device sends or receives
		SENDER,
		RECEIVER
	};
	enum Time_value { //amount of time that represents each button inputable character
		DOT = 500,
		DASH = 1000,
		END_CHAR = 2000,
		END_MSG = 5000
	};
	std::vector<char> message_char;
	bool input_next_morse_char;
	std::string mc_character; //stores the morse code string sequence
	std::string message; //The entire message per broadcast (e.g. "HELLO")
	bool broadcasting; // Whether the user has finished their message or not
	int role; //whether the device is marked to send or receive
	bool encrypt_message; //whether the message is encrypted or not
	bool pressed; //whether the button is pressed or not. Used for sending (as this devices button) and receiving (as the sending devices button / pin input)
};

#endif
