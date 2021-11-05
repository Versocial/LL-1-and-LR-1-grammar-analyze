#pragma once
#include<vector>
#include "../wordsAnalyze/word.h"
#include <istream>
#include <queue>
#include<unordered_map>
#include <set>

typedef std::vector<word> product;

class grammar
{
private:
	std::vector<product*> allProducts;//order in left symbol
	std::set<word>& nonTerminals = *new std::set<word>;
	std::unordered_map<std::string, int> firstIndex;

	std::vector<word> allSymbols;// without epsilon or dollor
	std::unordered_map<std::string, int> allIndexes;
	std::unordered_map<std::string, std::set<word>*>FIRSTset;
	std::unordered_map<std::string, std::set<word>*>FOLLOWset;
	static bool isOr(word w);
	//static word leftOf(product* p);
	void resetTerminals();//nonTerminals- firstIndex
	void resetSymbolIndex();// allSymbols-allIndexes
	void resetFIRSTset();//FIRSTset
	void resetFOLLOWset();//FOLLOWset
public:
	static word Epsilon();
	static word End();//$
	std::set<word>& N();
	int symbolNum();
	int index(word w);
	word alpha(int index);
	product& g(int i);
	int gsize()const;
	static void mergeNonEplisonIntoSet(std::set<word>& dest, std::set<word>& from);
	void firstOF(product& p, int begin,int end,std::set<word>& output);
	std::set<word>& FIRST(word n);
	std::set<word>& FOLLOW(word n);
	grammar(std::queue<word>& products);
	~grammar();
};
