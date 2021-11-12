
#include "DFA.h"


product& itemSet::g(item it)
{
	return gram->g(it.index);
}

bool itemSet::isReduce(item it)
{
	return g(it).size()==it.point+1;
}

bool itemSet::isShift(item it)
{
	return g(it).size()>it.point+1&&!gram->N().count( g(it)[it.point+1]);
}

bool itemSet::isToShift(item it)
{
	return g(it).size() > it.point + 1 && gram->N().count(g(it)[it.point+1]);
}

itemSet::itemSet(grammar& g)
{
	gram = &g;
	items = {};
	nextItem = {};
}

itemSet::~itemSet()
{
}

void itemSet::addItem(int index, int point, int look)
{
	item it;
	it.index = index;
	it.point = point;
	it.look = look;
	items.insert(it);
	return;
}

itemSet& itemSet::clousure()
{
	bool repeat = true;
	std::set<item>managed = {};
	while (managed.size()<items.size()) {
		int sizeBefore = items.size();
		for (std::set<item>::iterator i = items.begin(); i != items.end(); i++) {
			if (managed.count(*i))
				continue;
			else
				managed.insert(*i);
			if (isReduce(*i)||isShift(*i) || grammar::isEpsilonProduct(g(*i)))
				;
			else if (isToShift(*i)) {
				std::set<word> first;
				gram->firstOF(g(*i), (*i).point + 1, g(*i).size()-1, first);
				if (first.empty())
					first.insert(gram->alpha((*i).look));
				for (int j = gram->startOf(g(*i)[(*i).point + 1]);gram->g(j)[0]== g(*i)[(*i).point + 1]; j++) {
					item it ;
					it.index = j;
					it.point = 0;
					for (word w : first) {
						it.look = gram->index(w);
						items.insert(it);
					}
				}
			}
		}
		
	}
}

std::string itemSet::serializeInfo()
{
	std::string info = "[";
	for (item it : items) {
		info += it.serializeString() + ";";
	}
	return info+"]";
}
