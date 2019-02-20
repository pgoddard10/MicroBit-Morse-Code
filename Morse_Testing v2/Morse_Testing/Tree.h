#pragma once
#include <string>

class Tree
{
public:
	Tree();
	~Tree();
	bool insert(Tree* tree, char c, std::string morse_code);
	char find(Tree* tree, std::string morse_code);

private:
	Tree * dot;
	Tree * dash;
	char c;
};

