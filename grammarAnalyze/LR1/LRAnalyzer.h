#pragma once
#include "../grammar/grammar.h"
#include "DFA.h"
class LRAnalyzer {
private:
	grammar* gram;
	int tLine;
	int aRow;
	int gRow;
	std::vector<int> Action;
	std::vector<int> GoTo;
	int action(word n, word a);
	int goTo(word n, word a);

public:
	LRAnalyzer(grammar& gram);
	void setTableByLR1(DFA& dfa);
	void analyze(const std::queue<word>& input, std::queue<word>& output);
};