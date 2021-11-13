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
	//std::vector<item*>rowItems;
	std::unordered_map<std::string, itemSet*>nextItemSet;

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
	//void fixAllNext(const std::unordered_map<std::string, int>& setsIndex, std::vector<itemSet*>& sets);

};

class DFA {
private:
	product& g(item it);
		grammar* gram;
		//int startProductIndex;
		std::vector<itemSet*> sets;
		std::unordered_map<std::string ,int> setsIndex;
		void setGoTo(int i);
		//inline int newSet();
public:
	int index(itemSet& set);
	itemSet& theSet(int i);
	itemSet& startSet();
		DFA(grammar& g);
		~DFA();



};