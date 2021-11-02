#pragma once
#include<vector>
#include "../wordsAnalyze/word.h"
#include <istream>
#include <queue>
#include<unordered_map>

typedef std::vector<word> product;

class grammar
{
private:
	std::vector<product*> g;
	//std::
	//std::unordered_map<word, int> firstIndex;
	static bool isEnd(word w);
	static bool isOr(word w);
	static word leftOf(product p);
public:
	grammar(std::queue<word>& products);
	~grammar();
};
