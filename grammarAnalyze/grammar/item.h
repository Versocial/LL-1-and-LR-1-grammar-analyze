#pragma once
#include<string>
struct Item {
	int index;
	int point;
	int look;
	bool operator==(const struct Item& it) const {
		return index == it.index && point == it.point && look == it.look;
	}
	bool operator!=(const struct Item& it) const {
		return  index != it.index || point != it.point || look != it.look;
	}
	friend bool operator<(const struct Item it1, const struct Item& it2) {
		return it1.serializeString() < it2.serializeString();
	}
	std::string serializeString()const {
		std::string ans = std::to_string(index) + "." + std::to_string(point) + ":" + std::to_string(look);
		/*	for (std::set<int>::iterator i = look.begin(); i != look.end(); i++)
				ans += std::to_string(*i) + "|";*/
		return ans;
	}
};

typedef struct Item item;
