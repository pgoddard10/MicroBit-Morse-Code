#ifndef INTERFACE_H
#define INTERFACE_H

#include "MicroBit.h"
#include <string>
#include <vector>
#include <map>

class Interface
{
public:
	Interface();
	~Interface();
	void init(MicroBit* uBit);
	char get_human_char(std::vector<char>* message_n);
	ManagedString get_human_message();
	void send(MicroBit* uBit, uint64_t time);

private:
	enum Role {
		SENDER,
		RECEIVER
	};
	std::map<std::string, char> morse_to_human_char_map;
	std::vector<std::vector<char>> message;
	bool broadcasting;
	bool new_input_morse_char;
	bool error;
	uint8_t role;
	bool pressed;
};
#endif
