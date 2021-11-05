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
	std::set<word>N;
	std::vector< std::set<word>*>productFIRSTset;
	std::unordered_map<std::string, std::set<word>*>FIRSTset;
	std::unordered_map<std::string, std::set<word>*>FOLLOWset;
	//std::
	std::unordered_map<std::string, int> firstIndex;
	static bool isOr(word w);
	static word Epsilon();
	static word End();//$
	//static word leftOf(product* p);
	void setFIRSTset();
	void setFOLLOWset();
public:
	product& g(int i);
	int gsize();
	static void mergeNonEplisonIntoSet(std::set<word>& dest, std::set<word>& from);
	void firstOF(product& p, int begin,int end,std::set<word>& output);
	std::set<word>& FIRST(word n);
	std::set<word>& FOLLOW(word n);
	grammar(std::queue<word>& products);
	~grammar();
};
