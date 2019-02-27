/*
 * File: main.cpp
 * Author: Paul Goddard
 * Date: 20-02-2019
 */

#ifndef TREE_H
#define TREE_H

#include <string>

class Tree
{
public:
	Tree();
	~Tree();
	bool insert(Tree* tree, char c, std::string morse_code, std::string enc_morse_code = "~");
	char find(Tree* tree, std::string morse_code, std::string* enc_morse_code=nullptr);

private:
	Tree * dot; //Pointer to the dot/left node in the tree
	Tree * dash; //Pointer to the dash/right node in the tree
	char c; //the human-readable character (e.g. 'A')
	std::string enc_morse_code; //the encoded morse code string sequence for the above char
};

#endif
