#include "LRAnalyzer.h"


info LRAnalyzer::infoOf(char imply, int num)
{
	info in;
	in.imply = imply;
	in.num = num;
	return in;
}

info& LRAnalyzer::action(int n, word a)
{
	return table[n * (aRow + gRow) + gram->index(a)];
}

info& LRAnalyzer::goTo(int n, word a)
{
	return table[n * (aRow + gRow) + gram->index(a)];
}

LRAnalyzer::LRAnalyzer(grammar& gram)
{
	this->gram = &gram;
	gRow = gram.N().size();
	aRow = gram.gsize()+1 - gRow;
}

LRAnalyzer::~LRAnalyzer()
{
	if (table != NULL)
		delete[] table;
}

void LRAnalyzer::setTableByLR1()
{
	DFA dfa(*gram);
	tLine = dfa.size();
	if (table != NULL)
		delete[] table;
	table = new info[(aRow + gRow)*tLine];
	for (int i = 0; i < (aRow + gRow) * tLine; i++)
		table[i] = infoOf(errorImply, 0);

	for (int i = 0; i < dfa.size(); i++) {
		itemSet& itSet = dfa.theSet(i);
		for (item it : itSet.Items()) {
			if (gram->isReduce(it)) {
				action(i, gram->alpha(it.look)) = infoOf('R', it.index);
			}
			else if (gram->isShift(it)) {
				word w = gram->g(it.index)[it.point + 1];
				action(i,w ) = infoOf('S', dfa.index(*itSet.getNextItemSet(w)));
			}
			else if (gram->isToShift(it)) {
				word w = gram->g(it.index)[it.point + 1];
				goTo(i, w) = infoOf('G', dfa.index(*itSet.getNextItemSet(w)));
			}
		}
	}
	
}

void LRAnalyzer::analyze(const std::queue<word>& input, std::queue<word>& output)
{
}
