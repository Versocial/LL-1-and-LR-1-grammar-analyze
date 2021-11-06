#pragma once
#include<queue>
#include <ostream>
#include "BasicNode.h"
#include "BTNode.h"
#include "../wordsAnalyze/word.h"
class treeBuilder {
private:
	BasicNode* root;
	BasicNode* leftestNotLeaf();
public:
	void print(std::ostream& output);
	void buildBy(std::queue<word>& input);
};