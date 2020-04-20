
#include <iostream>
#include <fstream>
#include <cstring>
#include <unordered_map>
using namespace std;

#define NUM_TOKENS 10000


struct token{
    int code;
    string tag;
    string val;
};



void createHashTab();
void tokenize(char* file);
void classifyToken(string token);
void addToken(int code, string tag, string val);
bool isIdentifier(char c);
string getToken(string line);


void makeIndent();
bool match(int tokenCode);


bool parseClass();
bool parseClassVarDec();
bool parseSubroutineDec();
bool parseType(string func);
bool parseParameterList();
bool parseSubroutineBody();
bool parseVarDec();


bool parseStatements();
bool parseLetStatement();
bool parseDoStatement();
bool parseIfStatement();
bool parseWhileStatement();
bool parseReturnStatement();


bool parseExpression();
bool parseTerm();
bool parseOp();
bool parseSubroutineCall();
bool parseExpressionList();