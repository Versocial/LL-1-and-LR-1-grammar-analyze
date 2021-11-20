#pragma once
#include <vector>
#include<queue>
#include <stack>
#include <iostream>
#include "../grammar/grammar.h"

class LL1Analyzer {
private:
	int *predictAnalyzeTable;
	const int errorNum = -1;
	int analyzeWrong = false;
	grammar* gram=NULL;
	int Mline;
	int Mrow;
	void formTable();
	void error();
public:

	int M(word n, word a);
	LL1Analyzer( grammar&  gram);
	~LL1Analyzer();
	bool analyze(std::queue<word> input,std::deque<product*>& output);
};