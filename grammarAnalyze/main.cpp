
#include "wordsAnalyze/wordAnalyzer.h"
#include "treePrinter/BasicNode.h"
#include "treePrinter/BTNode.h"
#include "grammar/grammar.h"
#include "LL1/LL1.h"
#include<iostream>
#include<fstream>
#include<string>
using namespace::std;
int main()
{

    std::string path = "grammar/grammar.txt";
    ifstream input(path);
    if(!input.is_open()){
       std::cout << "Can Not Open File \"" << path << "\" .\n";
        return 1;
    }

    wordAnalyzer grammarAnalyzer(&input, std::cout);
    std::queue<word>grammarText;
    grammarAnalyzer.wordsAnalyze(grammarText);
    grammar g(grammarText);

    LL1Analyzer ll1(g);
    wordAnalyzer inputAnalyzer(&cin, std::cout);
    std::queue<word>inputText;
    inputAnalyzer.wordsAnalyze(inputText);
    ll1.analyze(inputText);

    return 0;
}