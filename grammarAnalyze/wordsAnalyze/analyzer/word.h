#pragma once
#include<string>

enum wordType {
	symbol,
	Bra,
	integer,
	realNum,
	end,
	string,
	charac,
	unkown
};

struct Word {
	std::string value;
	wordType type;
};

typedef struct Word word;

word wordOf(std::string value, wordType type);