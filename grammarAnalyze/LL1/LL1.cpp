#include "LL1.h"

int LL1Analyzer::M(word n, word a)
{
	return predictAnalyzeTable[ gram->index(n)*Mrow+gram->index(a)];
}

void LL1Analyzer::formTable()
{
	for (int j = 0; j < Mrow * Mline; j++) {
		predictAnalyzeTable[j] = errorNum;
	}
	for (int i = 0; i < gram->gsize(); i++) {
		product p = gram->g(i);
		std::set<word> first = {}; 
		gram->firstOF(p, 1, p.size() - 1, first);
		for (word w : first) {
			predictAnalyzeTable[gram->index(p[0]) * Mrow + gram->index(w)] = i;
		}
	}
}

LL1Analyzer::LL1Analyzer( grammar&  gramm)
{
	gram = &gramm;
	Mline = gram->N().size();
	Mrow = gram->symbolNum() - gram->N().size();
	predictAnalyzeTable= new int [Mline*Mrow];
	formTable();
}

LL1Analyzer::~LL1Analyzer()
{
	delete predictAnalyzeTable;
}

void LL1Analyzer::analyze(std::queue<word>& output)
{
}
