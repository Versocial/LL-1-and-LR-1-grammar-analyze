#pragma once
#include "../grammar/grammar.h"
#include "DFA.h"
#include <stack>

struct Info {
	char imply;
	int num;
};
typedef struct Info info;

class LRAnalyzer {
private:
	DFA* dfa;
	static const char errorImply = 'E';
	static const char accImply = 'A';
	static info infoOf(char imply, int num);
	grammar* gram;
	info* table=NULL;
	int tLine;
	int aRow;
	int gRow;
	bool analyzeWrong = false;

	info& action(int n, word a);
	info& goTo(int n, word a);
	void error();
public:
	void print(std::ostream& out);
	LRAnalyzer(grammar& gram);
	~LRAnalyzer();
	void setTableByLR1();
	bool analyze(std::queue<word> input, std::deque<product*>& output);
};