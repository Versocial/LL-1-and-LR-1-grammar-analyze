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
	int k = gram->index(a);
	if (n * (aRow + gRow) + gram->index(a) >= tLine * (aRow + gRow))
		std::cout << "[Error]: in action.\n";
	return table[n * (aRow + gRow) + gram->index(a)];
}

info& LRAnalyzer::goTo(int n, word a)
{
	if (n * (aRow + gRow) + gram->index(a) >= tLine * (aRow + gRow))
		std::cout << "[Error]: in goto.\n";
	return table[n * (aRow + gRow) + gram->index(a)];
}

void LRAnalyzer::error()
{
	std::cout << "[Error]: in LR !" << std::endl;
	analyzeWrong = true;
}

void LRAnalyzer::print(std::ostream& out)
{
	out << std::endl<<"The LR analyze table :\n";
	out << "\n<<=====Action=====>>" << std::endl;
	for (int i = 0; i < tLine; i++) {
		for (int j = gram->N().size(); j < gram->symbolNum(); j++)
			if (action(i, gram->alpha(j)).imply != errorImply)
				out << "action[ " << i << ", " << gram->alpha(j).value << "] = " << action(i, gram->alpha(j)).imply << action(i, gram->alpha(j)).num << std::endl;
	}
	out << "\n<<=====Goto=====>>" << std::endl;
	for (int i = 0; i < tLine; i++) {
		for (int j = 0; j < gram->N().size(); j++)
			if (goTo(i, gram->alpha(j)).imply != errorImply)
				out << "goto[ " << i << ", " << gram->alpha(j).value << "] = " << goTo(i, gram->alpha(j)).imply << goTo(i, gram->alpha(j)).num << std::endl;
	}
	std::cout << "\n";
}

LRAnalyzer::LRAnalyzer(grammar& gram)
{
	this->gram = &gram;
}

LRAnalyzer::~LRAnalyzer()
{
	delete dfa;
	if (table != NULL)
		delete[] table;
}

void LRAnalyzer::setTableByLR1()
{
	dfa = new DFA(*gram);
	gRow = gram->N().size();
	aRow = gram->symbolNum() - gRow;
	tLine = dfa->size();
	if (table != NULL)
		delete[] table;
	table = new info[(aRow + gRow) * tLine];
	for (int i = 0; i < (aRow + gRow) * tLine; i++)
		table[i] = infoOf(errorImply, 0);

	std::cout << "The DFA:\n";
	dfa->print(std::cout);
	std::cout << "\n";

	for (int i = 0; i < dfa->size(); i++) {
		itemSet& itSet = dfa->theSet(i);
		for (item it : itSet.Items()) {
			if (gram->isReduce(it)) {
				if (gram->g(it.index)[0] == gram->g(0)[0] && gram->g(it.index)[1] == gram->g(1)[0])//S'->S
					action(i, gram->alpha(it.look)) = infoOf(accImply, 0);
				else
					action(i, gram->alpha(it.look)) = infoOf('R', it.index);
				//std::cout << "action[" << i << "," << gram->alpha(it.look).value << "]=[R," << it.index << "]" << std::endl;
			}
			else if (gram->isShift(it)) {
				word w = gram->g(it.index)[it.point + 1];
				action(i, w) = infoOf('S', dfa->index(*itSet.getNextItemSet(w)));
				//std::cout << "action[" << i << "," << w.value << "]=[S," << dfa->index(*itSet.getNextItemSet(w)) << "]" << std::endl;
			}
			else if (gram->isToShift(it)) {
				word w = gram->g(it.index)[it.point + 1];
				goTo(i, w) = infoOf('G', dfa->index(*itSet.getNextItemSet(w)));
				//std::cout << "goto[" << i << "," << w.value << "]=[ " << dfa->index(*itSet.getNextItemSet(w)) << " ]" << std::endl;
			}
		}
	}
}

bool LRAnalyzer::analyze(std::queue<word> input, std::deque<product*>& output)
{
	analyzeWrong = false;
	std::stack<int> stateStack;
	std::stack<word> symbolStack;
	input.push(grammar::End());
	stateStack.push(0);
	std::cout << "\nAnalyzing start:\n";
	//std::cout<<dfa->printSet(0);
	while (true) {
		word w = input.front();
		if (w.type == wordType::integer || w.type == wordType::realNum)
			w = grammar::numFormatWord();
		//std::cout << "\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
		std::cout << "\nsymbol top: " << w.value<<std::endl;
		switch (action(stateStack.top(), w).imply)
		{
		case accImply:
			if (input.front() != grammar::End() ||  symbolStack.top() != gram->g(1)[0])
				error();
			input.pop();
			stateStack.pop();
			symbolStack.pop();
			if (!input.empty() || !symbolStack.empty() || !stateStack.size() == 1 || stateStack.top() != 0)
				error();
			return analyzeWrong;
			break;
		case 'R':
		{
			product& p = gram->g(action(stateStack.top(), w).num);
			output.push_back(&p);
			std::cout << "Reduce by " << grammar::printProduct(p) << std::endl;
			if (!grammar::isEpsilonProduct(p)) {
				for (int i = 1; i < p.size(); i++) {
					symbolStack.pop();
					stateStack.pop();
				}
			}
			symbolStack.push(p[0]);
			std::cout << ",back by " << stateStack.top() /*<< ":\n" << dfa->printSet(stateStack.top()) << std::endl*/;
			stateStack.push(goTo(stateStack.top(), symbolStack.top()).num);
			std::cout << ",goto " << stateStack.top() /*<< ":\n" << dfa->printSet(stateStack.top()) << std::endl*/;

		}
		break;
		case 'S':
			symbolStack.push(w);
			stateStack.push(action(stateStack.top(), w).num);
			std::cout << "Shift ["<<w.value<<"] to " << stateStack.top() /*<< ":\n" << dfa->printSet(stateStack.top()) << std::endl*/;
			input.pop();
			break;
		case errorImply:
			error();
			break;
		default:
			error();
			break;
		}
		if (analyzeWrong)
			break;
	}
	return analyzeWrong;
}
