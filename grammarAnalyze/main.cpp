//
//#include "wordsAnalyze/wordAnalyzer.h"
//#include "treePrinter/BasicNode.h"
//#include "treePrinter/BTNode.h"
//#include "grammar/grammar.h"
//#include "LL1/LL1.h"
//#include<iostream>
//#include<fstream>
//#include<string>
//#include "treePrinter/treeBuilder.h"
//using namespace::std;
//int main()
//{
//
//    std::string path = "grammar/grammar.txt";
//    ifstream input(path);
//    if(!input.is_open()){
//       std::cout << "Can Not Open File \"" << path << "\" .\n";
//        return 1;
//    }
//    std::string path2 = "grammar/input.txt";
//    ifstream input2(path2);
//    if (!input2.is_open()) {
//        std::cout << "Can Not Open File \"" << path2 << "\" .\n";
//        return 1;
//    }
//
//    wordAnalyzer grammarAnalyzer(&input, std::cout);
//    std::queue<word>grammarText = {};
//    grammarAnalyzer.wordsAnalyze(grammarText);
//    grammar g(grammarText);
//
//    LL1Analyzer ll1(g);
//    wordAnalyzer inputAnalyzer(&input2, std::cout);
//    std::queue<word>inputText = {};
//    std::deque<product*>output = {};
//    inputAnalyzer.wordsAnalyze(inputText);
//    bool analyzeWrong=ll1.analyze(inputText,output);
//    if (analyzeWrong) {
//        return 0;
//    }
//    treeBuilder tB;
//    tB.buildBy(output, g.N(),true);
//    //tB.print(cout);
//    tB.fixBy(inputText);
//    cout << "\nThe output:\n";
//    while (!output.empty()) {
//        cout << grammar::printProduct(*output.front())<<endl;
//        output.pop_front();
//    }
//    cout << "\nThe output in a tree:\n";
//    tB.print(cout);
//    return 0;
//}