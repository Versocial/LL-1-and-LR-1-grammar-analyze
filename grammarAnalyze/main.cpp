
#include "wordsAnalyze/wordAnalyzer.h"
#include "treePrinter/BasicNode.h"
#include "treePrinter/BTNode.h"
using namespace::std;
int main()
{
	BasicNode* head = new BasicNode("me");
	BasicNode* childL = new BasicNode("Paul");
	BasicNode* childCL = new BasicNode("Anna");
	BasicNode* childCC = new BasicNode("Sarah");
	BasicNode* childCR = new BasicNode("Emma");
	BasicNode* childR = new BasicNode("Tina");

	BasicNode* childLL = new BasicNode("Kevin");
	BasicNode* childLR = new BasicNode("Elloine");

	BasicNode* childCLC = new BasicNode("Anthony");

	BasicNode* childRL = new BasicNode("Susan");
	BasicNode* childRR = new BasicNode("Jason");

	head->addChildren(childL);
	head->addChildren(childCL);
	head->addChildren(childCC);
	head->addChildren(childCR);
	head->addChildren(childR);

	childCL->addChildren(childCLC);


	childL->addChildren(childLL);
	childL->addChildren(childLR);

	childR->addChildren(childRL);
	childR->addChildren(childRR);

	BTTree<BasicNode> printer(head, &BasicNode::getChildren, &BasicNode::getData);
	printer.print();

	delete head; // This deletes all subnodes
    // std::string path;
    // std::cin >> path;
    wordAnalyzer wordsAnalyzer(&std::cin, std::cout);
    //if(!wordsAnalyzer.fileOpened()){
    //    std::cout << "Can Not Open File \"" << path << "\" .\n";
    //     return 1;
    // }
    std::queue<word>result;
    wordsAnalyzer.wordsAnalyze(result);
    while (!result.empty()) {
        std::cout << (result.front()).value << " ";
        result.pop();
    }
    std::cout << wordsAnalyzer.compileInfos();
    std::cout << ">>>>>>  Words Analyze Complete !\n";
    system("pause");
    return 0;
}