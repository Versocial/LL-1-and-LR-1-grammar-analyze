
#include "wordsAnalyze/wordAnalyzer.h"
#include "treePrinter/BasicNode.h"
#include "treePrinter/BTNode.h"
#include "grammar/grammar.h"
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
    wordAnalyzer wordsAnalyzer(&input, std::cout);
    std::queue<word>result;
    wordsAnalyzer.wordsAnalyze(result);
    grammar g(result);


    return 0;
}