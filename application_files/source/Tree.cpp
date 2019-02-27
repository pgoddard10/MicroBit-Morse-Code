#include "Tree.h"
#include <string>

Tree::Tree() {
	//create a new Tree node setting everything to null
	dot = { nullptr };
	dash = { nullptr };
	c = '~';
	enc_morse_code = "";
}

Tree::~Tree() {
}

bool Tree::insert(Tree* tree, char c, std::string morse_code, std::string enc_morse_code) {
	//insert new data into the tree, creating a new leaf if needed.
	
	for (char mc : morse_code) { //loop through the morse code string (e.g. ".-.-")
		if (mc == '.') {
			if (tree->dot == nullptr)
				tree->dot = new Tree(); //create new leaf on the tree in the dot position
			tree = tree->dot;
		}
		else if (mc == '-') {
			if (tree->dash == nullptr)
				tree->dash = new Tree(); //create new leaf on the tree in the dash position
			tree = tree->dash;
		}
		else {
			return 1; //invalid input
		}
	}
	tree->c = c; //copy the char into the leaf
	tree->enc_morse_code = enc_morse_code; //copy the encrypted morse code string into the char
	return 0;
}

char Tree::find(Tree* tree, std::string morse_code, std::string* enc_morse_code) {
	for (char mc : morse_code) {
		if (mc == '.') {
			if (tree->dot == nullptr)
				return '*';
			tree = tree->dot;
		}
		else if (mc == '-') {
			if (tree->dash == nullptr)
				return '~';
			tree = tree->dash;
		}
		else {
			return '~'; //invalid input
		}
	}
	if(enc_morse_code!=nullptr) //i.e. if encryption is switched on
		*enc_morse_code = tree->enc_morse_code;
	return tree->c; //return the character
}
