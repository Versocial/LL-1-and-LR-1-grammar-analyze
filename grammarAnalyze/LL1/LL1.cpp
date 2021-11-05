#include "LL1.h"

int LL1Analyzer::M(word n, word a)
{
	return predictAnalyzeTable[gram->index(n) * Mrow + gram->index(a) - Mline];
}

void LL1Analyzer::formTable()
{
	int sum = 0;
	for (int j = 0; j < Mrow * Mline; j++) {
		predictAnalyzeTable[j] = errorNum;
	}
	for (int i = 0; i < gram->gsize(); i++) {
		product p = gram->g(i);
		std::set<word> first = {};
		gram->firstOF(p, 1, p.size() - 1, first);
		for (word w : first) {
			if (w != grammar::Epsilon()) {
				if (M(p[0], w) != errorNum)
					std::cout << "shit";
				predictAnalyzeTable[gram->index(p[0]) * Mrow + gram->index(w) - Mline] = i;
				sum++;
				std::cout << "\nM[" << p[0].value << "," << w.value << "]=" << i << std::endl;
			}
			else
				for (word w2 : gram->FOLLOW(p[0])) {
					if (M(p[0], w2) != errorNum)
						std::cout << "shit";
					predictAnalyzeTable[gram->index(p[0]) * Mrow + gram->index(w2) - Mline] = i;
					sum++;
					std::cout << "\nM[" << p[0].value << "," << w2.value << "]=" << i << std::endl;
				}
		}
	}
	std::cout << sum;
}

LL1Analyzer::LL1Analyzer(grammar& gramm)
{
	gramm.eliminateLeftR();
	gram = &gramm;
	Mline = gram->N().size();
	Mrow = gram->symbolNum() - gram->N().size();
	predictAnalyzeTable = new int[Mline * Mrow];
	formTable();
}

LL1Analyzer::~LL1Analyzer()
{
	delete[] predictAnalyzeTable;
}

void LL1Analyzer::analyze(std::queue<word>& output)
{
	predictAnalyzeTable[1];
}
