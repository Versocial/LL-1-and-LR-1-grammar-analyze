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
			int beforeSize = FIRST(p[0]).size();
			int k = 1;
			bool flag = true;
			while (flag) {
				if (!N.count(p[k])) {
					FIRST(p[0]).insert(p[k]);
					break;
				}
				else {
					mergeNonEplisonIntoSet(FIRST(p[0]), FIRST(p[k]));
				}
				flag = false;
				if (FIRST(p[k]).count(Epsilon()) && p.size() == k + 1) {
					FIRST(p[0]).insert(Epsilon());
				}
				else if (FIRST(p[k]).count(Epsilon()) && p.size() > k + 1) {
					flag = true; k++;
				}
			}
			if (beforeSize != FIRST(p[0]).size())
				repeat = true;
		}
	}
}

void grammar::setFOLLOWset()
{
	for (word symbol : N) {
		FOLLOWset[symbol.value] = new std::set<word>();
	}
	FOLLOW(g(0)[0]).insert(End());
	bool repeat = true;
	while (repeat) {
		repeat = false;
		for (int i = 0; i < gsize(); i++) {
			product p = g(i);

			for (int k = 1; k < p.size(); k++) {
				if (!N.count(p[k]))continue;
				int beforeSize = FOLLOW(p[k]).size();
				std::set<word> toMergeIn = {};
				firstOF(p, k + 1, p.size() - 1, toMergeIn);
				if (k == p.size() - 1 || toMergeIn.count(Epsilon()))
					mergeNonEplisonIntoSet(FOLLOW(p[k]), FOLLOW(p[0]));
				mergeNonEplisonIntoSet(FOLLOW(p[k]), toMergeIn);
				if (FOLLOW(p[k]).size() != beforeSize)
					repeat = true;
			}			
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

void grammar::mergeNonEplisonIntoSet(std::set<word>& dest, std::set<word>& from)
{
	bool hasEpsilon = from.count(Epsilon())&&!dest.count(Epsilon());
	dest.insert(from.begin(), from.end());
	if (hasEpsilon )
		dest.erase(Epsilon());
}

void grammar::firstOF(product& p, int begin, int end, std::set<word>& output)
{
	output.clear();
	if (begin >=p.size()||end<0||begin>end)return;
	int k = begin;
	bool repeat = true;
	while (repeat&&k<=end) {
		if (!N.count(p[k]))
			output.insert(p[k]);
		else 
			mergeNonEplisonIntoSet(output, FIRST(p[k]));
		
		if (!N.count(p[0]) && FIRST(p[k]).count(Epsilon()))
			repeat = true;
		else 
			repeat = false;
		k++;
	}
}


std::set<word>& grammar::FIRST(word n)
{
	return *(FIRSTset[n.value]);
}

std::set<word>& grammar::FOLLOW(word n)
{
	return *(FOLLOWset[n.value]);
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
	setFOLLOWset();
}

grammar::~grammar()
{
	for (std::vector<product*> ::iterator it = allProducts.begin(); it < allProducts.end(); it++) {
		delete (*it);
	}
}
