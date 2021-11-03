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
//
//word grammar::leftOf(product* p)
//{
//	return (*p)[0];
//}


void grammar::setFIRSTset()
{
	for (word symbol : N) {
		FIRSTset[symbol.value] = new std::set<word>();
	}
	bool repeat = true;
	while (repeat) {
		repeat = false;
		for (int i = 0; i < gsize(); i++) {
			product p = g(i);
			int beForeSize = FIRST(p[0]).size();
			int k = 1;
			bool flag = true;
			while (flag) {
				if (!N.count(p[k])) {
					FIRST(p[0]).insert(p[k]);
					break;
				}
				else {
					bool hasEpsilon = FIRST(p[0]).count(Epsilon());
					FIRST(p[0]).insert(FIRST(p[k]).begin(),FIRST(p[k]).end());
					if (!hasEpsilon && FIRST(p[0]).count(Epsilon()))
						FIRST(p[0]).erase(Epsilon());
				}
				flag = false;
				if (FIRST(p[k]).count(Epsilon()) && p.size() == k + 1) {
					FIRST(p[0]).insert(Epsilon());
				}
				else if (FIRST(p[k]).count(Epsilon()) && p.size() > k + 1) {
					flag = true; k++;
				}
			}
			if (beForeSize != FIRST(p[0]).size())
				repeat = true;
		}
	}
}

product& grammar::g(int i)
{
	return *allProducts[i];
}

int grammar::gsize()
{
	return allProducts.size();
}

std::set<word>& grammar::FIRST(word n)
{
	return *(FIRSTset[n.value]);
}

grammar::grammar(std::queue<word>& products)
{
	allProducts.clear();
	while(!products.empty()) {
		product* p = new std::vector<word>;
		p->clear();
		p->push_back(products.front());
		if ((*p)[0].type != wordType::identifier)return;//left must be identifier
		firstIndex[(*p)[0].value] = allProducts.size();
		N.insert((*p)[0]);
		products.pop();
		products.pop();//=>
		while (End()!=(products.front())) {
			if (isOr(products.front())) {
				allProducts.push_back(p);
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
		allProducts.push_back(p);
		products.pop();//$
	}

	setFIRSTset();
}

grammar::~grammar()
{
	for (std::vector<product*> ::iterator it = allProducts.begin(); it < allProducts.end(); it++) {
		delete (*it);
	}
}
