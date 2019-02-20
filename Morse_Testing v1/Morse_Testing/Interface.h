#pragma once
#include <iostream>
#include <vector>
#include <map>

class Interface
{
public:
	Interface();
	~Interface();
	void init();
	void build_message();
	char get_human_char(std::vector<char>* message_n);

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
	int role;
};

