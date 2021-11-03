#include "grammar.h"


bool grammar::isOr(word w)
{
	return w.type == wordType::symbol && w.value == "|";
}

word grammar::Epsilon()
{
	return wordOf("e", wordType::identifier);
}

word grammar::End()
{
	return wordOf("$",symbol);
}

word grammar::leftOf(product* p)
{
	return (*p)[0];
}


void grammar::setFIRST()
{
	for (word symbol : N) {
		FIRST[symbol.value] = new std::set<word>();
		for (int i = firstIndex[symbol.value]; i < g.size(); i++) {
			if (leftOf(g[i]) != symbol)break;
			if(!N.count((*g[i])[1]))FIRST[symbol.value]->insert();
		}
	}
}

grammar::grammar(std::queue<word>& products)
{
	g.clear();
	while(!products.empty()) {
		product* p = new std::vector<word>;
		p->clear();
		p->push_back(products.front());
		if ((*p)[0].type != wordType::identifier)return;//left must be identifier
		firstIndex[leftOf(p).value] = g.size();
		N.insert((*p)[0]);
		products.pop();
		products.pop();//=>
		while (End()!=(products.front())) {
			if (isOr(products.front())) {
				g.push_back(p);
				word left = (*p)[0];
				p = new std::vector<word>;
				p->clear();
				p->push_back(left);
			}
			else {
				p->push_back(products.front());
			}
			products.pop();
		}
		products.pop();//$
	}
}

grammar::~grammar()
{
	for (std::vector<product*> ::iterator it = g.begin(); it < g.end(); it++) {
		delete *it;
	}
}
