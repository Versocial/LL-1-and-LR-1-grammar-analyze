#pragma once
#include<vector>
#include "../wordsAnalyze/word.h"
#include <istream>
#include <queue>
#include<unordered_map>
#include <set>
#include "item.h"

typedef std::vector<word> product;

class grammar
{
private:
	std::vector<product*> allProducts;//order in left symbol

	std::set<word>& nonTerminals = *new std::set<word>;
	std::unordered_map<std::string, int> firstIndex;
	std::vector<word> allSymbols;// without epsilon or dollor, nonTerminal first
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
	static void mergeEliminatedInto(std::vector<product*>& dest, std::set<product*>&LR,std::set<product*>&notLR,word left);
	static word Epsilon();
	static word End();//$
	static word numFormatWord();//num
	std::set<word>& N();
	int symbolNum();
	int index(word w);
	word alpha(int index);
	product& g(int i);
	int gsize()const;
	static bool isEpsilonProduct(const product& p);
	static std::string printProduct(const product& p);
	static void mergeNonEplisonIntoSet(std::set<word>& dest, std::set<word>& from);
	void firstOF(product& p, int begin,int end,std::set<word>& output);
	std::set<word>& FIRST(word n);
	std::set<word>& FOLLOW(word n);
	int startOf(word w)const;
	grammar(std::queue<word>& products);
	void augment();
	void eliminateLeftR();
	~grammar();

	bool isReduce(item it)const;
	bool isShift(item it)const;
	bool isToShift(item it)const;
};
