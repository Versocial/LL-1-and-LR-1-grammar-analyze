#pragma once
#include <set>
#include<vector>
#include<unordered_map>
#include "../grammar/grammar.h"

struct Item {
	int index;
	int point;
	int look;
	bool operator==(const struct Item& it) const {
		return index==it.index&&point==it.point&& look == it.look;
	}
	bool operator!=(const struct Item& it) const {
		return  index != it.index || point != it.point || look != it.look;
	}
	friend bool operator<(const struct Item it1, const struct Item& it2) {
		return it1.serializeString() < it2.serializeString();
	}
	std::string serializeString()const {
		std::string ans = std::to_string(index) + "." + std::to_string(point) + ":"+std::to_string(look);
	/*	for (std::set<int>::iterator i = look.begin(); i != look.end(); i++)
			ans += std::to_string(*i) + "|";*/
		return ans;
	}
};

typedef struct Item item;

class itemSet {
private:
	std::set<item> items;
	grammar& gram;
	inline product& g(item it);
	//std::vector<item*>rowItems;
	bool isReduce(item it);
	bool isShift(item it);
	bool isToShift(item it);

public:
	std::unordered_map<int, int>nextItem;
	itemSet(grammar& g);
	~itemSet();
	void addItem(int index, int point,int look);
	itemSet& clousure();
	std::string serializeInfo();

};

class DFA {
		grammar& gram;
		int startProductIndex;
		std::vector<itemSet> sets;
		std::unordered_map<std::string ,int> setsIndex;
		DFA(grammar& g);
		~DFA();



};