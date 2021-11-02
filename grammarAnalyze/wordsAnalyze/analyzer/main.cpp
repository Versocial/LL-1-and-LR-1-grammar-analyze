#include "wordAnalyzer.h"
//using namespace::std;
int main()
{
   std::string path;
   std::cin >> path;
    wordAnalyzer wordsAnalyzer(path, std::cout);
   if(!wordsAnalyzer.fileOpened()){
       std::cout << "Can Not Open File \"" << path << "\" .\n";
        return 1;
    }
    wordsAnalyzer.wordsAnalyze();
    std::cout << wordsAnalyzer.compileInfos();
    std::cout << ">>>>>>  Words Analyze Complete !\n";
    system("pause");
    return 0;
}