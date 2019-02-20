#include "Tree.h"
#include <string>

Tree::Tree() {
	//create a new Tree node setting everything to null
	dot = { nullptr };
	dash = { nullptr };
	c = '\0';
}

Tree::~Tree() {
}

bool Tree::insert(Tree* tree, char c, std::string morse_code) {
	for (char mc : morse_code) {
		if (mc == '.') {
			if (tree->dot == nullptr)
				tree->dot = new Tree();
			tree = tree->dot;
		}
		else if (mc == '-') {
			if (tree->dash == nullptr)
				tree->dash = new Tree();
			tree = tree->dash;
		}
		else {
			return false; //invalid input
		}
	}
	tree->c = c;
	return true;
}

char Tree::find(Tree* tree, std::string morse_code) {
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
	return tree->c;
}
