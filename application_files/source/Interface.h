#ifndef INTERFACE_H
#define INTERFACE_H

#include "Tree.h"
#include <string>
#include <vector>

class Interface
{
public:
	Interface();
	~Interface();
	void run();
	void init();
	void error();
	void store_user_input(char user_input, Tree* tree);
	void mc_setup_next_char(char user_input, Tree* tree);
	void encrypt(char* c);
	void decrypt(char* c);
	void print_message();
	void send(char* c);
	void build_tree(Tree* tree);

private:
	enum Role {
		SENDER,
		RECEIVER
	};
	enum Time_value {
		DOT = 500,
		DASH = 1000,
		END_CHAR = 2000,
		END_MSG = 5000
	};
	std::vector<char> message_char;
	std::string mc_character;
	std::string message;
	bool broadcasting;
	int role;
	bool input_next_morse_char;
	bool encrypt_message;
	bool pressed;
};

#endif
