#include <string>
#include <iostream>

enum wordType{
	plus,
	sub,
	multiply,
	devide,
	leftBra,
	rightBra,
	num,
	end,
	unkown
};

struct Word {
	std::string value;
	std::string type;
};

typedef struct Word word;

word wordOf(std::string value, wordType type);

word getWord(std::istream& input);