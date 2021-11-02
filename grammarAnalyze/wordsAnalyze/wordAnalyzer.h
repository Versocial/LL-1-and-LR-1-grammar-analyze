#pragma once

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>
#include<queue>
#include <set>
#include "error.h"
#include "node.h"
#include "word.h"

class wordAnalyzer
{
private:
#define LENGTH 2048
    int lineNumber;
    long long  charNumber;
    long long lineStartNumber;

    static std::set<std::string>keyWord;
    static std::unordered_map<std::string, std::string> symbolMap;
    std::unordered_map<char, node*> symbolCheckTree = {};

    std::istream* input;
    std::ostream* output;
    char inputStr[LENGTH];
    char* nowChar, * forChar;
    bool bufferFlag;//true:12;false:21
    bool openFile;
    Error error;

    int noticedChar;
    int noticedString;
    int noticedIdentifier;
    int noticedSymbol;
    int noticedNumber;
    int noticedKeyWord;

    void initSymbolCheckTree();
    word symbolCheck();
    word numberCheck();
    word identifierCheck();
    word stringCheck();
    word charCheck();
    word pretreatCheck();
    void  ignoreAnnotation();

    inline long long nowCharNum();
    int incForChar();

public:
    wordAnalyzer(std::istream*input, std::ostream& out);
    ~wordAnalyzer();
    bool fileOpened();
    void wordsAnalyze(std::queue<word>& result);
    void errorInfoAppend(int type, std::string detail);
    std::string compileInfos();

};
