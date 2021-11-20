#pragma once
#include <set>
#include<vector>
#include<unordered_map>
#include "../grammar/grammar.h"
#include "../grammar/item.h"
#include<iostream>

class itemSet {
private:
	std::set<item> items;
	grammar* gram;
	inline product& g(item it);
	std::unordered_map<std::string, itemSet*>go;

public:
	void setNextItemSet(word , itemSet*);
	itemSet* getNextItemSet(word);
	std::set<item> Items()const;
	int size()const;
	itemSet(grammar& g);
	~itemSet();
	void addItem(int index, int point,int look);
	itemSet* clousure();
	std::string serializeInfo();
	std::string printInfo(std::unordered_map<std::string, int>&setsIndex);
	
};

class DFA {
private:
	product& g(item it);
		grammar* gram;
		std::vector<itemSet*> sets;
		std::unordered_map<std::string ,int> setsIndex;
		void setGo(int i);
public:
	int index(itemSet& set);
	int size();
	itemSet& theSet(int i);
	itemSet& startSet();
		DFA(grammar& g);
		~DFA();
	std::string printSet(int i);
	void print(std::ostream& out);

};