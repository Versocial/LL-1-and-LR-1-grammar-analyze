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
	identifier,
	key,
	unkown,
	empty
};

struct Word {
	std::string value;
	wordType type;
};

typedef struct Word word;

word wordOf(std::string value, wordType type);