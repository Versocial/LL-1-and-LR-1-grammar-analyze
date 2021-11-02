#include "wordAnalyzer.h"
//using namespace::std;


void wordAnalyzer::wordsAnalyze(std::queue<word>& result)
{

    input->read(inputStr, LENGTH / 2 - 1);
    inputStr[LENGTH / 2 - 1] = inputStr[LENGTH - 1] = EOF;
    nowChar = forChar = inputStr;
    bufferFlag = false;
    word w;

    while (*nowChar) {
        //w .clear();
        w = wordOf("", wordType::empty);
        if (isdigit(*nowChar)) {
            w = numberCheck();
        }
        else if (isalpha(*nowChar) || *nowChar == '_') {
            w = identifierCheck();
        }
        else if (*nowChar == '/') {
            if (Error::EndOfFile == incForChar()) { errorInfoAppend(Error::UnfinishedToken, "/"); break; }
            else if (*forChar == '*' || *forChar == '/') { nowChar = forChar; ignoreAnnotation(); }
            else { forChar = nowChar; w = symbolCheck(); }
        }
        else if (isspace(*nowChar)) {
            if (*nowChar == '\n') {
                //w += '\n';
                lineNumber++;
                lineStartNumber = nowCharNum();
            }
        }
        else if (*nowChar == '\"') {
            w = stringCheck();
        }
        else if (*nowChar == '\'') {
            w = charCheck();
        }
        else if (symbolCheckTree.count(*nowChar)) {
            w = symbolCheck();
        }
        else if (*forChar == '#') {
           // w = "\n"+pretreatCheck();
            w = pretreatCheck();
        }
        else {
            std::string info = "";
            while (!isspace(*forChar)&&';'!=(*forChar)) {
                info += *forChar;
                if (Error::EndOfFile == incForChar())break;
            };
            errorInfoAppend(Error::UnkownInput, "\"" + info + "\"");
            w = wordOf(info,wordType::unkown);           

        };
        if (w.type != wordType::empty) result.push(w);
        if (Error::EndOfFile == incForChar()) {
            break;
        }
        nowChar = forChar;
    }
    return;

}


std::unordered_map<std::string, std::string> wordAnalyzer::symbolMap = {
            {"+","plus"},{"++","inc"}, {"+=","plusAssign"},
            {"-","sub"},{"--","dec"},{"-=","decAssign"}, {"->","target"},
            {"*","star"},{"*=","multiplyAssign" },
            {"/","devide"},{"/=","devideAssign"},
            {"%","reminder"}, {"%=","reminderAssign"},
            {"<","lessThan"},{"<=","lessEql"},{"<<","leftShift"},{"<<=","leftShiftAssign"},
            {">","greaterThan"},{">=","greaterEql"},{">>","rightShift"},{">>=","rightShiftAssign"},
            {"=","assign"},{"==","equal"},
            {"!","not"},{"!=","notEql" },
            {"~","bitNot"},{"~=","bitNotAssign" },
            {"^","bitXor"},{"^=","bitXorAssign"},
            {"&","bitAnd"},{"&=","bitAndAssign"},{"&&","and"},
            {"|","bitOr"},{"|=","bitOrAssign"},{"||","or"},
            {"(","-"},{")","-"},{"[","-"},{"]","-"},{"{","-"},{"}","-"},
            {".","point"},{"?","question"},{":","colon"},{";","semicolon"},{",","comma"},
            {"=>","arrow"},{"$","end"}//use only in chan sheng shi
};//这里原义键值对的值是读取到键后记录的记号

void wordAnalyzer::initSymbolCheckTree() {
    for (std::unordered_map<std::string, std::string>::iterator it = symbolMap.begin(); it != symbolMap.end(); it++) {
        node* now = NULL;
        for (char ch : it->first) {
            if (now == NULL) {
                if (symbolCheckTree[ch] == NULL)symbolCheckTree[ch] = new node(ch);
                now = symbolCheckTree[ch];
            }
            else {
                node* temp = now->seekSon(ch);
                if (temp != NULL) {
                    now = temp;
                }
                else {
                    now = now->newSon(ch);
                }
            }

        }

    }
}

word wordAnalyzer::symbolCheck()
{
    forChar = nowChar;
    node* now = symbolCheckTree[*nowChar];
    std::string symbol = ""; symbol += *nowChar;
    while (now->hasSon()) {
        char* temp = forChar;
        if (Error::EndOfFile == incForChar())return wordOf(symbol, wordType::unkown);
        now = now->seekSon(*forChar);
        if (now == NULL) { forChar = temp; break; }
        symbol += *forChar;
    }
    noticedSymbol++;
    return  wordOf(symbol,wordType::symbol);//"<"+symbol+","+symbolMap[symbol]+">";
}

word wordAnalyzer::numberCheck()
{
    forChar = nowChar;
    int state = 0;
    std::string number = "";
    char* temp = forChar;
    bool Exit = false;
    
    while (!Exit) {
        switch (state)
        {
        case 0:
            if (isdigit(*forChar));
            else if (*forChar == '.')state = 1;
            else if (*forChar == 'f') state = 3;
            else if (*forChar == 'E' || *forChar == 'e')state = 12;
            else if (*forChar == 'x') {
                if (number != "0")Exit = true;
                if (Error::EndOfFile == incForChar()) {
                    Exit = true;
                }
                else if (isdigit(*forChar)) {
                    number += 'x';
                    state = 4;
                }
                else {
                    Exit = true;
                }
            }
            else Exit = true;
            break;
        case 1:
            if (isdigit(*forChar));
            else if (*forChar == 'E' || *forChar == 'e')state = 2;
            else if (*forChar == 'f')  state = 3;
            else Exit = true;
            break;
        case 2://float
        case 12://integer
            if (isdigit(*forChar));
            else if ((*forChar == '+' || *forChar == '-') && (*temp == 'E' || *temp == 'e'));
            else if (*forChar == 'f') state = 3;
            else Exit = true;
            break;
        case 3:
            Exit = true;
            break;
        case 4:
            if (isxdigit(*forChar));
            else Exit = true;
            break;
        default://forChar = temp;
            errorInfoAppend(Error::UnkownState, "numberCheck DFA unkown state  when manage\"" + number + "\"");
            Exit = true;
            break;
        }

        if (!Exit) {
            number += *forChar;
            temp = forChar;
            if (Error::EndOfFile == incForChar()) {
                forChar = temp; Exit = true;
            }
        }
        else forChar = temp;
    }
    word w;
    switch (state) {
    case 0:case 12:case 4:w =wordOf (number, wordType::integer); number = "<integer," + number + ">"; noticedNumber++; break;
    case 1:case 2:case 3:w = wordOf(number, wordType::realNum); number = "<realNumber," + number+">"; noticedNumber++; break;
    default:w = wordOf(number, wordType::unkown); number = "<?number," + number+">"; break;
    }
    return w;
}



std::set<std::string> wordAnalyzer::keyWord = {
    "int", "long", "short", "float", "double", "char",
    "unsigned", "signed", "const", "void", "volatile", "enum", "struct", "union",
    "if", "else", "goto", "switch", "case", "do", "while", "for", "continue", "break", "return", "default", "typedef",
    "auto", "register", "extern", "static",
    "sizeof"
};

word wordAnalyzer::identifierCheck()
{
    forChar = nowChar;
    int state = 0;
    std::string identifier = "";
    char* temp = forChar;
    while (1) {
        if (isalpha(*forChar) || *forChar == '_' || (isdigit(*forChar) && identifier.size())) {
            identifier += *forChar;
            temp = forChar;
            if (Error::EndOfFile == incForChar()) { forChar = temp; break; }
        }
        else { forChar = temp; break; }
    }
    
    if (keyWord.count(identifier)) {
        noticedKeyWord++;
        return wordOf(identifier, wordType::key);
    }
    else {
        noticedIdentifier++;
        return wordOf(identifier, wordType::identifier);
    }
}


word wordAnalyzer::stringCheck()
{
    forChar = nowChar;
    int state = 0;
    bool Exit = false;
    std::string ans = "";
    while (!Exit) {
        switch (state)
        {
        case 0:
            if ('\"' == *forChar) {
                ans += *forChar;
                state = 1;
            }
            else {
                errorInfoAppend(Error::UnfinishedString, ans); 
                Exit = true; 
            }
            break;
        case 1:
            if ('\\' == *forChar) { ans += *forChar; state = 2; }
            else if ('\"' == *forChar) { ans += *forChar; Exit = true; }
            else if ('\n' == *forChar||!isprint(*forChar)) { 
                Exit = true; 
                errorInfoAppend(Error::UnfinishedString, ans);
               if('\n'==*forChar) lineNumber++;
            }
            else ans += *forChar;
            break;
        case 2:
            if (*forChar == '\n') { ans.pop_back(); lineNumber++; }
            else if (!isprint(*forChar)) {
                Exit = true;
                errorInfoAppend(Error::UnfinishedString, ans);
            }
            else ans += *forChar;
            state = 1;
            break;
        default:
            break;
        }               
        if (!Exit && Error::EndOfFile == incForChar()) { errorInfoAppend(Error::UnfinishedString,ans); break; }
    }
    if (state == 1) {
        noticedString++; return wordOf(ans, wordType::string); }
    else return wordOf(ans,wordType::unkown);
}


word wordAnalyzer::charCheck() {
    forChar = nowChar;
    int state = 0;
    std::string ch = "";
    char* temp = forChar;
    bool Exit = false;
    while (!Exit) {
        switch (state) {
        case -1:break;
        case 0:
            if (*forChar == '\'')state = 1;
            else { state = -1; Exit = true; }
            break;
        case 1:
            if (*forChar == '\\')state = 2;
            else if (isprint(*forChar) && *forChar != '\n')state = 4;
            else { state = -1; Exit = true; }
            break;
        case 2:
            if (isalpha(*forChar) || *forChar == '\'' || *forChar == '\"' || *forChar == '\\')state = 3;
            else { state = -2; Exit = true; }
            break;
        case 3:
            if (*forChar == '\'')
                Exit = true;
            else
                state = -2;
            break;
        case 4:
            if (*forChar == '\'')
                Exit = true;
            else if (*forChar == '\\')
                state = -3;
            else 
                state = -2;
            break;
        case -2:
            if (*forChar == '\'')
                Exit = true;
            else if (*forChar == '\\')
                state = -3;
            else;
            break;
        case -3:
            state = -2;
            break;
        default:Exit = true; break;
        }
        ch += *forChar;
        if (!Exit) {
            temp = forChar;
            if (Error::EndOfFile == incForChar()) { errorInfoAppend(Error::UnfinishedChar, ch); forChar = temp; Exit = true; }
        }
        else {
            if (*forChar != '\''){
                forChar = temp;
                ch.pop_back();
            }
            if (state != 4 && state != 3) {
                if (state == -2)errorInfoAppend(Error::UnsupportedChar, ch);
                else errorInfoAppend(Error::UnfinishedChar,ch);
            }            
        }
    }
    if (state == 4 || state == 3) {
        noticedChar++;    return wordOf(ch,wordType::charac);
    }
    else return wordOf(ch,wordType::unkown);
}

void wordAnalyzer::ignoreAnnotation()
{
    forChar = nowChar;
    int state = 1;
    bool Exit = false;
    char* temp = forChar;
    while (!Exit) {
        switch (state) {
        case 1:
            if (*forChar == '/')state = 2;
            else if (*forChar == '*')state = 4;
            else return;
        case 2:
            if (*forChar == '\n')Exit = true;
            break;
        case 4:
            if (*forChar == '*')state = 5;
            else if (*forChar == '\n')lineNumber++;
            break;
        case 5:
            if (*forChar == '/')Exit = true;
            else if (*forChar == '*');
            else state = 4;
            break;
        }
        if (!Exit) {
            temp = forChar;
            if (Error::EndOfFile == incForChar()) {
                forChar = temp; Exit = true;
            }
        }
        else if (state != 5) forChar = temp;
    }
    if (state != 2 && state != 5) { errorInfoAppend(Error::UnfinishedAnnotation, ""); }
    return;
}

word wordAnalyzer::pretreatCheck()
{
    char* temp=forChar;
    while ('\n' != (*forChar)) {
        temp = forChar;
        if (Error::EndOfFile == incForChar())break;
    };
    std::string info=std::string(nowChar,forChar-nowChar);
    nowChar = forChar=temp;
    return wordOf(info,empty);
}