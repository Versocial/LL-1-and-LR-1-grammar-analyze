#include "grammar.h"

bool grammar::isEnd(word w)
{
	return w.type == wordType::symbol&&w.value=="$";
}

bool grammar::isOr(word w)
{
	return w.type == wordType::symbol && w.value == "|";
}

bool grammar::isEpsilon(word w)
{
	return w.type == wordType::identifier && w.value == "e";
}

std::string grammar::leftOf(product p)
{
	return p[0].value;
}

grammar::grammar(std::queue<word>& products)
{
	g.clear();
	while(!products.empty()) {
		product* p = new std::vector<word>;
		firstIndex[leftOf(*p)] = g.size();
		p->clear();
		p->push_back(products.front());
		products.pop();
		products.pop();//=>
		while (!isEnd(products.front())) {
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
