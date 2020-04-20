
#include <iostream>
#include <fstream>
#include <cstring>
#include <unordered_map>
using namespace std;

#define NUM_TOKENS 10000

#define KEY_class         101
#define KEY_constructor   102
#define KEY_function      103
#define KEY_method        104
#define KEY_field         105
#define KEY_static        106
#define KEY_var           107
#define KEY_int           108
#define KEY_char          109
#define KEY_boolean       110
#define KEY_void          111
#define KEY_true          112
#define KEY_false         113
#define KEY_null          114
#define KEY_this          115
#define KEY_let           116
#define KEY_do            117
#define KEY_if            118
#define KEY_else          119
#define KEY_while         120
#define KEY_return        121

#define SYM_curly_open    201
#define SYM_curly_close   202
#define SYM_par_open      203
#define SYM_par_close     204
#define SYM_sqr_open      205
#define SYM_sqr_close     206
#define SYM_dot           207
#define SYM_comma         208
#define SYM_semi_colon    209
#define SYM_add           210
#define SYM_sub           211
#define SYM_mul           212
#define SYM_div           213
#define SYM_and           214
#define SYM_or            215
#define SYM_less_than     216
#define SYM_greater_than  217
#define SYM_equal         218
#define SYM_not           219

#define CONST_int         300
#define CONST_string      400
#define IDENTIFIER        500



enum scope {STATIC, FIELD, ARG, LOCAL};
enum type {INT, CHAR, BOOLEAN, CLASSNAME};

struct token{
    int code;
    string tag;
    string val;
};

struct entry{
    string type;
    string scope;
    string index;
};



void createHashTab();
void tokenize(char* file);
void classifyToken(string token);
void addToken(int code, string tag, string val);
bool isIdentifier(char c);
string getToken(string line);


void makeIndent();
bool match(int tokenCode);


bool parse_Class();
bool parse_ClassVarDec();
bool parse_SubroutineDec();
bool parse_Type(string callingFunction);
bool parse_ParameterList();
bool parse_SubroutineBody();
bool parse_VarDec();


bool parse_Statements();
bool parse_LetStatement();
bool parse_DoStatement();
bool parse_IfStatement();
bool parse_WhileStatement();
bool parse_ReturnStatement();


bool parse_Expression();
bool parse_Term();
bool parse_Op();
bool parse_SubroutineCall();
bool parse_ExpressionList();

void ST_addEntry(string name, string type, int scope);
void ST_clear();
struct entry* ST_search(string var);