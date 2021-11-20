#include "treeBuilder.h"

BasicNode* treeBuilder::leftestNotLeaf(BasicNode* node)//node must can be not leaf
{
	if (node != nullptr) {
		if (node->getChildren().size() == 0)
			return node;
		std::list<BasicNode*> nlist = node->getChildren();
		for (std::list<BasicNode*>::iterator now =nlist.begin(); now != nlist.end(); now++) {
			if ((*now)->isLeaf() == BasicNode::mustLeaf)
				continue;
			BasicNode* ans= leftestNotLeaf(*now);
			if (ans != nullptr)
				return ans;
		}
	}
	else 
		return nullptr;
}

BasicNode* treeBuilder::firstNotLeaf(bool leftest)
{
	if (leftest)
		return leftestNotLeaf(root);
	else
		return rightestNotLeaf(root);
}

BasicNode* treeBuilder::rightestNotLeaf(BasicNode* node)
{
	if (node != nullptr) {
		if (node->getChildren().size() == 0)
			return node;
		std::list<BasicNode*> nlist = node->getChildren();
		for (auto now = nlist.rbegin(); now != nlist.rend(); now++) {
			if ((*now)->isLeaf() == BasicNode::mustLeaf)
				continue;
			BasicNode* ans = rightestNotLeaf(*now);
			if (ans != nullptr)
				return ans;
		}
	}
	else
		return nullptr;
}

treeBuilder::treeBuilder()
{
	root = nullptr;
}

treeBuilder::~treeBuilder()
{
	delete root;
}

void treeBuilder::buildBy(std::deque<product*> input,std::set<word>N, bool leftest)
{


	if (root != nullptr)
		delete root;
	root =new BasicNode( (*input.front())[0],BasicNode::canNotLeaf);
	
	void (std::deque<product*>:: * pop)(); 
	product*& (std::deque<product*>:: * top)();
	if (leftest) {
		pop = &std::deque<product*>::pop_front;
		top = &std::deque<product*>::front;
	}
	else {
		pop = &std::deque<product*>::pop_back;
		top = &std::deque<product*>::back;
	}
	;

	while (!input.empty()) {
		product p = *((& input)->*top)();
		((&input)->*pop)();
		BasicNode* now = firstNotLeaf(leftest);
		if (now==nullptr||p[0] != now->Word())
			std::cout << "[Error]:in tree building:the nonTerminal should be "<<p[0].value << " instead of " << now->Word().value<<std::endl;
		for (int i = 1; i < p.size(); i++) {
			int isLeaf = N.count(p[i]) ? BasicNode::canNotLeaf : BasicNode::mustLeaf;
			now->addChildren(new BasicNode(p[i], isLeaf));
		}
	}
	if (firstNotLeaf(leftest) != nullptr)
		std::cout << "[Error]:in tree building:the tree should be finished but not" << std::endl;
	return;
}

void treeBuilder::print(std::ostream& output)
{
	BTTree<BasicNode> printer(root, &BasicNode::getChildren, &BasicNode::getData);
	printer.print();

}

void treeBuilder::fixBy(std::queue<word> input)
{
	fixSomeBy(root, input);
}

void treeBuilder::fixSomeBy(BasicNode* now, std::queue<word>& input)
{
	if (now == nullptr)
		return;
	if (now->getChildren().size() == 0) {
		if (now->Word() == grammar::numFormatWord()) {
			word w = input.front();
			w.value = "num:" + w.value;
			now->setWord(w);
		}
		if (now->Word() != grammar::Epsilon())
			input.pop();
		else
			now->setWord(wordOf("[epsilon]", wordType::identifier));
	}
	else {
		std::list<BasicNode*> nlist = now->getChildren();
		for (std::list<BasicNode*>::iterator it = nlist.begin(); it != nlist.end(); it++) {
			fixSomeBy(*it, input);
		}
	}
	return;
}
