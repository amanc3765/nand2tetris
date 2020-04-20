
#include <iostream>
#include <fstream>
#include <cstring>
#include <unordered_map>
using namespace std;

#define CODE_LEN  50000
#define LABEL_LEN 10000
#define PART_LEN  20



struct A_ins{
    string label;
};

struct C_ins{
    int type;
    string dest;
    string comp; 
    string jump; 
};



string clean(string command);
string dec_bin(int n);
string transInsA(struct A_ins ins_A);
string transInsC(struct C_ins ins_C);
void createHashTab();
void parse(string commandList[], int commandNum);
void assemble(string commandList[], int commandNum);
void createSymbolTab(string labelList[], int labelAdd[], int labelNum, string commandList[], int commandNum);
int compare(char *file1);




