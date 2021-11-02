#pragma once
#include<string>
enum wordType {
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
	wordType type;
};

typedef struct Word word;

word wordOf(std::string value, wordType type) {
	word w;
	w.type = type;
	w.value = value;
	return w;
}