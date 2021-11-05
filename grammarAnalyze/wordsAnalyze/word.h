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
	bool operator==(const struct Word& w) const {
		return type == w.type && value == w.value;
	}
	bool operator!=(const struct Word& w) const {
		return type != w.type || value != w.value;
	}
	friend bool operator<(const struct Word w1,const struct Word& w2) { 
		return std::to_string(w1.type)+ w1.value < std::to_string(w2.type) + w2.value;
	}
	std::string serializeString() {
		return std::to_string(type) +":"+ value;
	}
};


typedef struct Word word;


word wordOf(std::string value, wordType type);