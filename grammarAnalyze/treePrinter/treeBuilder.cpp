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

BasicNode* treeBuilder::leftestNotLeaf()
{
	/*if (root==nullptr||root->getChildren().size()==0)
		return root;	*/
	return leftestNotLeaf(root);
}

treeBuilder::treeBuilder()
{
	root = nullptr;
}

treeBuilder::~treeBuilder()
{
	delete root;
}

void treeBuilder::buildBy(std::queue<product*> input,std::set<word>N)
{
	if (root != nullptr)
		delete root;
	root =new BasicNode( (*input.front())[0],BasicNode::canNotLeaf);
	while (!input.empty()) {
		product p = *input.front();
		input.pop();
		BasicNode* now = leftestNotLeaf();
		if (now==nullptr||p[0] != now->Word())
			std::cout << "shit";
		for (int i = 1; i < p.size(); i++) {
			int isLeaf = N.count(p[i]) ? BasicNode::canNotLeaf : BasicNode::mustLeaf;
			now->addChildren(new BasicNode(p[i], isLeaf));
		}
	}
	if (leftestNotLeaf() != nullptr)
		std::cout << "shit2";
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
			now->setWord(wordOf(" ε ", wordType::identifier));
	}
	else {
		std::list<BasicNode*> nlist = now->getChildren();
		for (std::list<BasicNode*>::iterator it = nlist.begin(); it != nlist.end(); it++) {
			fixSomeBy(*it, input);
		}
	}
	return;
}
