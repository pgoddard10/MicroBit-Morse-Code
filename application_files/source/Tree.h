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
	Tree * dot;
	Tree * dash;
	char c;
	std::string enc_morse_code;
};

#endif
