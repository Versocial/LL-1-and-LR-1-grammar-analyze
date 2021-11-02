#include "wordAnalyzer.h"
//using namespace::std;

word wordOf(std::string value,wordType type){
word w;
w.type = type;
w.value = value;
return w;
}
wordAnalyzer::wordAnalyzer(std::string path, std::ostream& out) {
    error = Error();
    noticedChar = 0;
    noticedString = 0;
    noticedIdentifier = 0;
    noticedSymbol = 0;
    noticedNumber = 0;
    noticedKeyWord = 0;
    lineNumber = 1;
    charNumber = 0;
    lineStartNumber = 0;
    initSymbolCheckTree();
   std:: ifstream* inputs = new std:: ifstream(path);
    openFile = inputs->is_open();
    if (openFile)input = inputs;
    output = &out;
}

wordAnalyzer::~wordAnalyzer()
{
    for (std::unordered_map<char, node*>::iterator it = symbolCheckTree.begin(); it != symbolCheckTree.end(); it++) {
        delete it->second;
    };
    if(openFile)((std::ifstream*)input)->close();
}

bool wordAnalyzer::fileOpened()
{
    return openFile;
}

std::string wordAnalyzer::compileInfos()
{
    return
        "\n\n\t\t\t\t<<<<<======== [ Information ] ========>>>>>\t\t\t\t\n\t\t\t\t     " +
       std:: to_string(lineNumber) + " lines | " + std::to_string(charNumber) + " characters | " + std::to_string(error.errorNum()) + " errors.\n\t\t" +
        "Noticed : "+ std::to_string(noticedIdentifier)+" identifiers | " + std::to_string(noticedKeyWord)+" keyWords | "+ std::to_string(noticedSymbol)+" symbols | " + std::to_string(noticedChar)+" chars | " + std::to_string(noticedString) +" strings | " + std::to_string(noticedNumber)+" numbers \n" +
        error.getInfo();
}


int wordAnalyzer::incForChar()
{
    forChar = &inputStr[(forChar - inputStr + 1) % LENGTH];
    if (*forChar == EOF) {
        if (&inputStr[LENGTH / 2 - 1] == forChar) {
            if (bufferFlag == false) {
                if (nowChar > &inputStr[LENGTH / 2 - 1]) { return Error::TooLongToken; }
                input->read(&inputStr[LENGTH / 2], LENGTH / 2 - 1);
                bufferFlag = true;
            }
            forChar = &inputStr[LENGTH / 2];
        }
        else if (&inputStr[LENGTH - 1] == forChar) {
            if (bufferFlag == true) {
                if (nowChar < &inputStr[LENGTH / 2 - 1]) { return Error::TooLongToken; }
                input->read(&inputStr[0], LENGTH / 2 - 1);
                charNumber += LENGTH - 2;
                bufferFlag = false;
            }
            forChar = &inputStr[0];
        }
    }
    else if (*forChar == '\0') {
        charNumber = nowCharNum();
        return Error::EndOfFile;
    }

    return Error::NoError;
}


inline long long  wordAnalyzer::nowCharNum() {
    return charNumber + (forChar - inputStr >= LENGTH / 2) ? forChar - inputStr  : forChar - inputStr +1;
}

void wordAnalyzer::errorInfoAppend(int type, std::string detail)
{
    error.addInfo(lineNumber, nowCharNum()-lineStartNumber, type, detail);
}

