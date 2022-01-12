#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <map>
#include <iomanip>

#define MAX_ID_LEN 64
#define MAX_KEYWORD_LEN 31
#define MAX_SYMBOL_TABLE_LEN 512
#define MAX_NUMERIC_TABLE_LEN 512
#define KEYWORD_TABLE_LEN 34
#define MAX_BUFFER_LEN 256

char *symbol_table[MAX_SYMBOL_TABLE_LEN];
char *numeric_table[MAX_NUMERIC_TABLE_LEN];
const char keyword[KEYWORD_TABLE_LEN][MAX_KEYWORD_LEN] = {
    "auto", "break", "case", "char", "const", "continue",
    "default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "inline", "int",
    "long", "register", "restrict", "return", "short", "signed",
    "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while"};

using namespace std;

map<int, string> name; 

class LexAnalysis{
public:
    int state;
    int lineNum;
    int charNum;
    int idNum;
    int numNum;
    int keyWordNum;
    int otherNum;
    int charconstNum;
    int stringconstNum;

    ifstream cfile;

    LexAnalysis(){
        state = 0;
        AddName();
    }
    void AddName();
    bool ReadCFile(const char *fileName);

    void StartAnalysis();
    char StepOverSpace();
    bool IsLetter(const char c);
    bool IsDownLine(const char c);
    bool IsNumeric(const char c);
    bool IsPlusPipeAmpGreaterLess(const char c);
    bool IsMinus(const char c);
    bool IsTildeExclaimStarPercentEqualCaret(const char c);
    bool IsSlash(const char c);
    bool IsDoubleQquotationMark(const char c);
    bool IsQquotationMark(const char c);
    bool IsKeyword(const char *word);
    bool LexComment();
};

void LexAnalysis::AddName(){
    
    name['('] = "l_paren";
    name[')'] = "r_paren";
    name['{'] = "l_brace";
    name['}'] = "r_brace";
    name['['] = "l_square";
    name[']'] = "r_square";
    name[';'] = "semi";
    name[','] = "comma";
    name[':'] = "colon";
    name['#'] = "hash";
    name['?'] = "question";
    name['+'] = "plus";
    name['-'] = "minus";
    name['*'] = "star";
    name['/'] = "slash";
    name['='] = "equal";
    name['|'] = "pipe";
    name['&'] = "amp";
    name['>'] = "greater";
    name['<'] = "less";
    name['!'] = "exclaim";
    name['~'] = "tilde";
    name['*'] = "star";
    name['%'] = "precent";
    name['^'] = "caret";
    name['.'] = "point";
}

bool LexAnalysis::ReadCFile(const char *fileName){
    bool is_open = false;
    cfile.open(fileName, ios::in | ios::binary);
    if(cfile.is_open()){
        is_open = true;
    }
    return is_open;
}

bool LexAnalysis::IsLetter(const char c){
    bool isLetter = false;
    if( (c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A') )
        isLetter = true;
    return isLetter;
}

bool LexAnalysis::IsDownLine(const char c){
    bool isDownLine = false;
    if( c == '_' )
        isDownLine = true;
    return isDownLine;
}

bool LexAnalysis::IsPlusPipeAmpGreaterLess(const char c)
{
    bool isPlusPipeAmp = false;
    if (c == '+' || c == '|' || c == '&' || c == '<' || c == '>')
        isPlusPipeAmp = true;
    return isPlusPipeAmp;
}

bool LexAnalysis::IsMinus(const char c)
{
    bool isMinus = false;
    if (c == '-')
        isMinus = true;
    return isMinus;
}

char LexAnalysis::StepOverSpace()
{   
    char c;
    cfile >> noskipws  >> c;

    while ( cfile.peek() != EOF && (c == ' ' || c == '\n' || c == '\t' || c == '\r')){
        if (c == '\n')
            lineNum++;
        cfile >> c;
    }
    return c;
}

bool LexAnalysis::IsNumeric(const char c)
{
    bool isNumeric = false;
    if( c <= '9' && c >= '0')
        isNumeric = true;
    return isNumeric;
} 

bool LexAnalysis::IsKeyword(const char *word)
{
    bool isKeyword = false;
    int len = 0;
    int wordLen = strlen(word);
    for(len = 0;len < KEYWORD_TABLE_LEN;len++){
        if(strlen(keyword[len]) == wordLen && strncmp(word,keyword[len],wordLen) == 0){
            isKeyword = true;
            break;
        }
    }
    return isKeyword;
}

bool LexAnalysis::IsTildeExclaimStarPercentEqualCaret(const char c){
    bool isTildeExclaimStarPercentEqualCaret = false;
    if (c == '!' || c == '~' || c == '*' || c == '%' || c == '=' || c == '^')
        isTildeExclaimStarPercentEqualCaret = true;
    return isTildeExclaimStarPercentEqualCaret;
}

bool LexAnalysis::IsQquotationMark(const char c){
    bool isQquotationMark = false;
    if (c == '\''){
        isQquotationMark = true;
    }
    return isQquotationMark;
}

bool LexAnalysis::IsDoubleQquotationMark(const char c){
    bool isDoubleQquotationMark = false;
    if (c == '\"')
    {
        isDoubleQquotationMark = true;
    }
    return isDoubleQquotationMark;
}

bool LexAnalysis::IsSlash(const char c){
    bool isSlash = false;
    if( c == '/' )
        isSlash = true;
    return isSlash;
}

bool LexAnalysis::LexComment(){
    int len = 0;
    char c;

    cfile >> noskipws >> c;
    while (cfile.peek() != EOF ){
        while (c != '*' && cfile.peek() != EOF)
        {
            cfile >> noskipws >> c;
            if (c == '\n')
                lineNum++;
            len++;
        }
        cfile >> noskipws >> c;
        len++;

        if( c == '/' ){
            break;
        }
        cfile >> noskipws >> c;
        len++;
    }
    if( c != '/'){
        cfile.seekg(-len,ios_base::cur);
        return 0;
    }
    return 1;
}

void LexAnalysis::StartAnalysis(){
    char c;
    int len = 0;
    lineNum = 1;
    charNum = 0;

    idNum = 0;
    numNum = 0;
    keyWordNum = 0;

    charconstNum = 0;
    stringconstNum = 0;
    otherNum = 0;

    string before, now;
    char buffer[MAX_BUFFER_LEN] = {'\0'};

    while ( 1 ){
        switch (state){
            case 0:
                charNum += len;
                memset(buffer, 0, MAX_BUFFER_LEN);
                len = 0;
                c = StepOverSpace();

                //return because of EOF
                if ( c == ' ' || c == '\n' || c == '\t' || c == '\r' ) return ;
                else if( IsLetter(c) || IsDownLine(c) ) state = 1;
                else if( IsNumeric(c) ) state = 2;
                else if( IsPlusPipeAmpGreaterLess(c) ) state = 8;
                else if( IsMinus(c) ) state = 9;
                else if( IsTildeExclaimStarPercentEqualCaret(c) ) state = 10;
                else if( IsSlash(c) ) state = 11;
                else if( IsQquotationMark(c) ) state = 12;
                else if( IsDoubleQquotationMark(c) ) state = 13;
                else if( (c == '(' || c == ')' || c == '{' || c == '}' || c == ',' || c == ';' || c == '#' || c == ':' || c == '?' || c == '[' || c == ']' || c == '.' ) ){
                    cout << "<" << name[c];
                    otherNum++;
                    state = 0xFF;
                }else{
                    cout << "!!! ERROR UNKNOW " << "   ' " << c << " ' !!!" << endl;
                }
                buffer[len++] = c;
                if( cfile.peek() == EOF){
                    if( state != 0xFF ){
                        cout << "<" << name[c];
                        otherNum++;
                    }
                    
                    cout << " , " << buffer << "> line:" << lineNum << endl;
                    charNum += len;
                    return;
                }
                break;
            case 1:
                cfile >> noskipws >> c;
                if ( (IsLetter(c) || IsDownLine(c) || IsNumeric(c)) && !cfile.eof()){
                    state = 1;
                    buffer[len++] = c;
                }
                else{
                    if( !IsKeyword(buffer) ) {
                        cout << "<Identifier";
                        idNum++;
                    }
                    else{
                        cout << "<" << buffer;
                        keyWordNum++;
                    }
                    cfile.seekg(-1,ios_base::cur);
                    state = 0xFF;
                }
                break;
            case 2:
                cfile >> noskipws >> c;
                if( cfile.peek() != EOF){
                    if( IsNumeric(c) )state = 2;
                    else if( c == '.' ) state = 3;
                    else if( c == 'E' || c == 'e') state = 5;
                    else{
                        cout << "<Numeric";
                        numNum++;
                        cfile.seekg(-1, ios_base::cur);
                        state = 0xFF;
                    }
                }else{
                    cout << "<Numeric";
                    buffer[len++] = c;
                    numNum++;
                    state = 0xFF;
                }
                
                if(state != 0xFF)
                    buffer[len++] = c;
                break;
            case 3:
                cfile >> noskipws >> c;
                if( IsNumeric(c) ) state = 4;
                buffer[len++] = c;
                break;
            case 4:
                cfile >> noskipws >> c;
                if( IsNumeric(c) ) state = 4;
                if( cfile.peek() == EOF){
                    state = 0xFF;
                    break;
                }
                else if( (c == 'E' || c == 'e')) state = 5;
                else{
                    cfile.seekg(-1, ios_base::cur);
                    cout << "<Numeric";
                    numNum++;
                    state = 0xff;
                }
                if( state != 0xFF ) buffer[len++] = c;
                break;
            case 5:
                cfile >> noskipws >> c;
                if( c == '+' || c == '-' ) state = 6;
                else if( IsNumeric(c) ) state = 7;
                else {
                    cout << "!!! ERROR UNKNOW " << "   ' " << buffer << " ' !!!" << endl;
                    state = 0;
                }
                if (cfile.peek() == EOF){
                    charNum += len;
                    return;
                }
                buffer[len++] = c;
                break;
            case 6:
                cfile >> noskipws >> c;
                if( IsNumeric(c) ) state = 7;
                buffer[len++] = c;
                break;
            case 7:
                cfile >> noskipws >> c;
                if( IsNumeric(c) ){
                    state = 7;
                    buffer[len++] = c;
                }else{
                    state = 0xFF;
                    cfile.seekg(-1, ios_base::cur);
                    cout << "<Numeric";
                    numNum++;
                }
                break;
            case 8:
                before = name[buffer[len - 1]];
                cfile >> noskipws >> c;
                if( buffer[len-1] == '+' && c == '+' ) buffer[len++] = c;
                else if( buffer[len-1] == '|' && c == '|') buffer[len++] = c;
                else if ( buffer[len-1] == '&' && c == '&'  ) buffer[len++] = c;
                else if (buffer[len - 1] == '<' && c == '<') buffer[len++] = c;
                else if (buffer[len - 1] == '>' && c == '>') buffer[len++] = c;
                else if (c == '=') buffer[len++] = c;
                else{
                    cfile.seekg(-1, ios_base::cur);
                    cout << "<" << before << " , " << buffer << "> line:" << lineNum << endl;
                    otherNum++;
                }
                if(len == 2){
                    now = name[c];
                    cout << "<" << before << now << " , " << buffer << "> line:" << lineNum << endl;
                    otherNum++;
                }
                if (cfile.peek() == EOF){
                    charNum += len;
                    return;
                }
                state = 0;
                break;
            case 9:
                before = "minus";
                cfile >> noskipws >> c;
                if( c == '-'  ){
                    buffer[len++] = c;
                    now = "minus";
                }
                else if(c == '>' ){
                    buffer[len++] = c;
                    now = "greater(arrow)";
                }
                else if(c == '='){
                    buffer[len++] = c;
                    now = "equal";
                }else{
                    cfile.seekg(-1, ios_base::cur);
                }
                if(len == 2){
                    cout << "<" << before << now << " , " << buffer << "> line:" << lineNum << endl;
                    otherNum++;
                }
                else{
                    cout << "<" << before << " , " << buffer << "> line:" << lineNum << endl;
                    otherNum++;
                }
                
                if (cfile.peek() == EOF){
                    charNum += len;
                    return;
                }
                state = 0;
                break;
            case 10:
                before = name[buffer[len-1]];
                cfile >> noskipws >> c;
                if( c == '='  ){
                    buffer[len++] = c;
                }else{
                    cfile.seekg(-1,ios_base::cur);
                }
                if( len == 2 ){
                    cout << "<" << before << name['='] << " , " << buffer << "> line:" << lineNum << endl;
                    otherNum++;
                }else{
                    cout << "<" << before << " , " << buffer << "> line:" << lineNum << endl;
                    otherNum++;
                }
                
                if (cfile.peek() == EOF)
                    return;
                state = 0;
                break;
            case 11:
                before = name[buffer[len - 1]];
                cfile >> noskipws >> c;
                if( c == '/' ){
                    lineNum += 1;
                    cfile >> noskipws >> c;
                    if( cfile.peek() == EOF ) break;
                    while ( c!= '\n' && cfile.peek() != EOF){
                        cfile >> noskipws >> c;
                    }
                    
                }
                else if( c == '*' ){
                    if( !LexComment() ){
                        cout << "!!! missing terminating \" */ \" character !!!" << endl;
                        if (cfile.peek() == EOF)
                            return;
                    } 
                }
                else if(c == '=' ){
                    now = name[c];
                    buffer[len++] = c;
                    cout << "<" << before << name[c] << " , " << buffer << "> line:" << lineNum << endl;
                    otherNum++;
                }else{
                    cout << "<" << before << " , " << buffer << "> line:" << lineNum  << endl;
                    otherNum++;
                    cfile.seekg(-1, ios_base::cur);
                }
                if (cfile.peek() == EOF)
                    return;
                state = 0;
                break;
            case 12:
                cfile >> noskipws >> c;
                while( c != '\r' && c != '\n' && cfile.peek() != EOF && c != '\'' ){
                    buffer[len++] = c;
                    cfile >> noskipws >> c;
                }
                if( c == '\'' ){
                    buffer[len++] = c;
                    cout << "<const_character" << " , " << buffer << "> line:" << lineNum  << endl;
                    charconstNum++;
                }else if( c == '\n' || c == '\r' || cfile.peek() == EOF ){
                    cout << "!!! missing terminating \" \' \" character !!!" << endl;
                    lineNum++;
                    cout << "!!! ERROR UNKNOW " << "   ' " << buffer << " ' !!!" << endl;
                }
                if (cfile.peek() == EOF)
                    return;
                state = 0;
                break;
            case 13:
                cfile >> noskipws >> c;
                while( c != '\r' && c != '\n' && cfile.peek() != EOF && c != '\"'){
                    buffer[len++] = c;
                    cfile >> noskipws >> c;
                }
                if( c == '\"'){
                    buffer[len++] = c;
                    cout << "const_string" << " , " << buffer << "> line:" << lineNum  << endl;
                    stringconstNum++;
                    if (cfile.peek() == EOF)
                        return;
                }else if( c == '\n' || c == '\r' || cfile.peek() == EOF){
                    cout << "missing terminating \" \" \" character" << endl;
                    lineNum++;
                    cout << "ERROR UNKNOW" << "   ' " << buffer << " ' !!! " << endl;
                    if (cfile.peek() == EOF)
                        return;
                }
                state = 0;
                break;
            case 0xFF:
                cout << " , " << buffer << "> line:" << lineNum << endl;
                state = 0;
                if( cfile.peek() == EOF ){
                    charNum += len;
                    return ;
                }
                break;
        }
    }

}

int main()
{
    const char fileName[] = "test.c";

    LexAnalysis lex;
    if( !lex.ReadCFile(fileName) ){
        cout << "Open " << fileName << " failed!";
        return 0;
    }
    lex.StartAnalysis();
    cout << "LineNum : " << lex.lineNum << endl;
    cout << "CharNum : " << lex.charNum << endl;
    cout << "idNum : " << lex.idNum << endl;
    cout << "KeywordNum" << lex.keyWordNum << endl;
    cout << "numNum : " << lex.numNum << endl;

    cout << "CharConstNum : " << lex.charconstNum << endl;
    cout << "stringConstNum : " << lex.stringconstNum << endl;
    cout << "otherNum : " << lex.otherNum << endl;

    system("pause");
    return 0;
}
