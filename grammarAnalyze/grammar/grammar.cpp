#include "grammar.h"


bool grammar::isOr(word w)
{
	return w.type == wordType::symbol && w.value == "|";
}

void grammar::mergeEliminatedInto(std::vector<product*>& dest, std::set<product*>& LR, std::set<product*>& notLR, word left)
{
	if (LR.empty()) { 
		for (product* p : notLR) 
			dest.push_back(new product(*p));
		return;
	}
	word newLeft = wordOf(left.value + "^", left.type);
	for (product* p : notLR) {		
		product* newP = new std::vector<word>(*p);
		if (p->size() == 2 && (*p)[1] == Epsilon()) {
			(*newP)[1] = newLeft;
		}
		newP->push_back(newLeft);
		dest.push_back(newP);
	}
	for (product* p : LR) {
		product* newP = new std::vector<word>(*p);
		(*newP)[0] = newLeft;
		for (int i = 1; i < newP->size()-1; i++) {
			(*newP)[i] = (*newP)[i + 1];
		}
		(*newP)[newP->size() - 1] = newLeft;
		dest.push_back(newP);
	}
	product* newP = new std::vector<word>();
	newP->push_back(newLeft);
	newP->push_back(Epsilon());
	dest.push_back(newP);
}

word grammar::Epsilon()
{
	return wordOf("e", wordType::identifier);
}

word grammar::End()
{
	return wordOf("$", symbol);
}

word grammar::numFormatWord()
{
	return wordOf("num", wordType::identifier);
}


void grammar::resetTerminals()
{
	nonTerminals.clear();
	firstIndex.clear();
	for (int i = 0; i < gsize(); i++) {
		product p = g(i);
		if (firstIndex.count(p[0].serializeString()))
			continue;
		else {
			nonTerminals.insert(p[0]);
			firstIndex[p[0].serializeString()] = i;
		}
	}
}

void grammar::resetSymbolIndex()
{
	allIndexes.clear();
	allSymbols.clear();
	for (int i = 0; i < gsize(); i++) {// nonTerminal first
		if (allIndexes.count(g(i)[0].serializeString()))
			continue;
		allIndexes[g(i)[0].serializeString()] = allSymbols.size();
		allSymbols.push_back(g(i)[0]);
	}

	for (int i = 0; i < gsize(); i++)
		for (int k = 1; k < g(i).size(); k++) {
			if (allIndexes.count(g(i)[k].serializeString()) || g(i)[k] == Epsilon())
				continue;
			allIndexes[g(i)[k].serializeString()] = allSymbols.size();
			allSymbols.push_back(g(i)[k]);
		}
	allIndexes[End().serializeString()] = allSymbols.size();
	allSymbols.push_back(End());
}

void grammar::resetFIRSTset()
{
	FIRSTset.clear();
	for (word symbol : nonTerminals) {
		FIRSTset[symbol.serializeString()] = new std::set<word>();
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
				if (!nonTerminals.count(p[k])) {
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

void grammar::resetFOLLOWset()
{
	FOLLOWset.clear();
	for (word symbol : nonTerminals) {
		FOLLOWset[symbol.serializeString()] = new std::set<word>();
	}
	FOLLOW(g(0)[0]).insert(End());
	bool repeat = true;
	while (repeat) {
		repeat = false;
		for (int i = 0; i < gsize(); i++) {
			product p = g(i);

			for (int k = 1; k < p.size(); k++) {
				if (!nonTerminals.count(p[k]))continue;
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

std::set<word>& grammar::N()
{
	return nonTerminals;
}

int grammar::symbolNum()
{
	return allSymbols.size();
}

int grammar::index(word w)
{
	return allIndexes[w.serializeString()];
}

word grammar::alpha(int index)
{
	return allSymbols[index];
}



product& grammar::g(int i)
{
	return *(allProducts[i]);
}

int grammar::gsize()const
{
	return allProducts.size();
}

bool grammar::isEpsilonProduct( const product& p)
{
	return p.size()==2&&p[1]==Epsilon();
}

void grammar::mergeNonEplisonIntoSet(std::set<word>& dest, std::set<word>& from)
{
	bool hasEpsilon = from.count(Epsilon()) && !dest.count(Epsilon());
	dest.insert(from.begin(), from.end());
	if (hasEpsilon)
		dest.erase(Epsilon());
}

std::string grammar::printProduct(const product& p)
{
	std::string ans = p[0].value + " =>";
	for (unsigned int i = 1; i < p.size(); i++) {
		if (p[i] == Epsilon())
			ans = ans + " ε";
		else
			ans = ans+" "+p[i].value;
	}
	return ans;
}

void grammar::firstOF(product& p, int begin, int end, std::set<word>& output)
{
	output.clear();
	if (begin >= p.size() || end<0 || begin>end)return;
	int k = begin;
	bool repeat = true;
	while (repeat && k <= end) {
		if (!nonTerminals.count(p[k]))
			output.insert(p[k]);
		else
			mergeNonEplisonIntoSet(output, FIRST(p[k]));

		if (nonTerminals.count(p[k]) && FIRST(p[k]).count(Epsilon()))
			repeat = true;
		else
			repeat = false;
		k++;
	}
	if(repeat&&k>end)
		output.insert(Epsilon());
}


std::set<word>& grammar::FIRST(word n)
{
	return *(FIRSTset[n.serializeString()]);
}

std::set<word>& grammar::FOLLOW(word n)
{
	return *(FOLLOWset[n.serializeString()]);
}

grammar::grammar(std::queue<word>& products)
{
	allProducts.clear();
	allSymbols.clear();

	while (!products.empty()) {
		product* p = new std::vector<word>;
		p->clear();
		p->push_back(products.front());
		if ((*p)[0].type != wordType::identifier)return;//left must be identifier
		firstIndex[(*p)[0].serializeString()] = allProducts.size();
		nonTerminals.insert((*p)[0]);
		products.pop();
		products.pop();//=>
		while (End() != (products.front())) {
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

	resetSymbolIndex();
	resetFIRSTset();
	resetFOLLOWset();
}

std::string grammar::printProduct(int k)
{
	std::string ans = g(k)[0].value + " => ";
	for (int i = 1; i < g(k).size(); i++)
		ans += g(k)[i].value+" ";
	return ans;
}

void grammar::eliminateLeftR()
{
	if (allProducts.size() == 0)return;
	std::vector<product*>tempG(allProducts);
	allProducts.clear();
	std::set<product*> sameLeftProductsNotLR = {};
	std::set<product*>sameLeftProductsLR = {};//left recursices

	for (word left : nonTerminals) {

		for (int i = firstIndex[left.serializeString()];
			i<tempG.size()&& (*tempG[i])[0]==left;
			i++) {
			if ((*tempG[i])[0] \
				== (*tempG[i])[1])
				sameLeftProductsLR.insert(tempG[i]);
			else
				sameLeftProductsNotLR.insert(tempG[i]);
		}
		mergeEliminatedInto(allProducts, sameLeftProductsLR, sameLeftProductsNotLR, left);
		sameLeftProductsLR.clear();
		sameLeftProductsNotLR.clear();
	}
	for (product* p : tempG)
		delete p;
	resetTerminals();
	resetSymbolIndex();
	resetFIRSTset();
	resetFOLLOWset();
}

grammar::~grammar()
{
	delete& nonTerminals;
	for (std::vector<product*> ::iterator it = allProducts.begin(); it < allProducts.end(); it++) {
		delete (*it);
	}
}

int grammar::startOf(word w)
{
	return firstIndex[w.serializeString()];
}

void grammar::augment()
{
	allProducts.push_back(allProducts[allProducts.size() - 1]);
	for (int i = allProducts.size() - 1; i > 0; i--)
		allProducts[i] = allProducts[i - 1];
	allProducts[0] = new std::vector<word>();
	allProducts[0]->push_back(wordOf("S\'", identifier));
	allProducts[0]->push_back((*allProducts[1])[0]);
	resetTerminals();
	resetSymbolIndex();
	resetFIRSTset();
	resetFOLLOWset();
}

bool grammar::isReduce(item it)const
{
	return (*allProducts[it.index]).size() == it.point + 1||isEpsilonProduct(*allProducts[it.index]);
}

bool  grammar::isShift(item it)const
{
	return (*allProducts[it.index]).size() > it.point + 1 && !nonTerminals.count((*allProducts[it.index])[it.point + 1]);
}

bool  grammar::isToShift(item it)const
{
	return (*allProducts[it.index]).size() > it.point + 1 && nonTerminals.count((*allProducts[it.index])[it.point + 1]);
}