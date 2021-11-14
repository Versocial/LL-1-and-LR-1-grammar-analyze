
#include "DFA.h"

product& itemSet::g(item it)
{
	return gram->g(it.index);
}


void itemSet::setNextItemSet(word k, itemSet* v)
{
	nextItemSet[k.serializeString()] = v;
}

itemSet* itemSet::getNextItemSet(word k)
{
	if (nextItemSet.count(k.serializeString()))
		return nextItemSet[k.serializeString()];
	else return NULL;
}

std::set<item> itemSet::Items() const
{
	return items;
}


int itemSet::size() const
{
	return items.size();
}

itemSet::itemSet(grammar& g)
{
	gram = &g;
	items = {};
	nextItemSet = {};
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

itemSet* itemSet::clousure()
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
			if (gram->isReduce(*i)|| gram->isShift(*i) )
				;
			else if (gram->isToShift(*i)) {
				std::set<word> first;
				gram->firstOF(g(*i), (*i).point + 2, g(*i).size()-1, first);
				if (first.empty())
					first.insert(gram->alpha((*i).look));
				for (int j = gram->startOf(g(*i)[(*i).point + 1]);j<gram->gsize()&&gram->g(j)[0]== g(*i)[(*i).point + 1]; j++) {
					item it ;
					it.index = j;
					it.point = 0;
					for (word w : first) {
						it.look = gram->index(w);
						//if(!items.count(it))
						//	std::cout << gram->printProduct(it.index) << " "<< gram->alpha(it.look).value <<" " << it.index << " " << it.point << " " << it.look << std::endl;
						items.insert(it);
						
					}
				}
			}
		}
		
	}
	std::cout << " \n";
	return this;
}

std::string itemSet::serializeInfo()
{
	std::string info = "[";
	
	for (const item it : items) {
		info += it.serializeString() + ";";		
	}
	return info+"]";
}


std::string itemSet::printInfo(std::unordered_map<std::string, int>&setsIndex)
{
	std::string ans;
	for (item it : items) {
		ans += gram->printProduct(it.index) + " " + gram->alpha(it.look).value + "\t " +std::to_string( it.index) + " " +std::to_string(it.point) + " " +std::to_string(it.look)+"\n";
	}
	for (auto i = nextItemSet.begin(); i != nextItemSet.end(); i++) {
		ans += "goto " + (*i).first + " = " + std::to_string(setsIndex[(*i).second->serializeInfo()])+"\n";
	}
	return ans;
}


void DFA::setGoTo(int k)
{
	std::set<item> theSet = (*sets[k]).Items();
	std::set<word>nexts = {};
	for (item it :theSet) {
		if (gram->isReduce(it))
			continue;
		else if (gram->isShift(it)||gram->isToShift(it)) {
			if ((*sets[k]).getNextItemSet(g(it)[it.point + 1]) ==NULL) {
				(*sets[k]).setNextItemSet(g(it)[it.point + 1], new itemSet(*gram));
				nexts.insert(g(it)[it.point + 1]);
			}
			(*sets[k]).getNextItemSet(g(it)[it.point + 1])->addItem(it.index, it.point + 1, it.look);
		}	
	}
	for (word w : nexts) {

		(*sets[k]).getNextItemSet(w)->clousure();
		if (setsIndex.count((*(*sets[k]).getNextItemSet(w)).serializeInfo())) {
			itemSet* temp= (*sets[k]).getNextItemSet(w);
			(*sets[k]).setNextItemSet(w, sets[setsIndex[(*(*sets[k]).getNextItemSet(w)).serializeInfo()]]);
			delete temp;
			//std::cout << "^^^ the clousure " << index(*(*sets[k]).getNextItemSet(w)) << std::endl;
		}
		else {
			sets.push_back((*sets[k]).getNextItemSet(w));
			setsIndex[(*(*sets[k]).getNextItemSet(w)).serializeInfo()]=sets.size()-1;
			//std::cout << "^^^ the clousure " << index(*(*sets[k]).getNextItemSet(w)) << std::endl;
			setGoTo(sets.size() - 1);
		}
	}
}
int DFA::index(itemSet& set)
{
	return setsIndex[set.serializeInfo()];
}
int DFA::size()
{
	return sets.size();
}
itemSet& DFA::theSet(int i)
{
	return *sets[i];
}

itemSet& DFA::startSet()
{
	return *sets[0];
}

DFA::DFA(grammar&g)
{
	gram = &g;
	g.augment();
	sets.push_back(new itemSet(g));
	sets[0]->addItem(0, 0, gram->index(grammar::End()));
	sets[0] = sets[0]->clousure();
	setGoTo(0);
	print(std::cout);
	
}

DFA::~DFA()
{
	for (int i = 0; i < sets.size(); i++) {
		delete sets[i];
	}
}

std::string DFA::printSet(int i)
{
	return sets[i]->printInfo(setsIndex);
}

void DFA::print(std::ostream& out)
{
	for (int i = 0; i < sets.size(); i++) {
		out << "\nitemSet " << i << std::endl;
		out << sets[i]->printInfo(setsIndex);
		
	}

}

product& DFA::g(item it)
{
	return gram->g(it.index);
}