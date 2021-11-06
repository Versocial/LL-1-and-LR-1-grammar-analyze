#pragma once
#include<queue>
#include <ostream>
#include "BasicNode.h"
#include "BTNode.h"
#include "../wordsAnalyze/word.h"
#include "../grammar/grammar.h"
class treeBuilder {
private:
	BasicNode* root;
	BasicNode* leftestNotLeaf(BasicNode* node);
	BasicNode* leftestNotLeaf();
	void fixSomeBy(BasicNode* now,std::queue<word>& input);
public:
	treeBuilder();
	~treeBuilder();
	void print(std::ostream& output);
	void buildBy(std::queue<product*> input, std::set<word>N);
	void fixBy(std::queue<word> input);

};