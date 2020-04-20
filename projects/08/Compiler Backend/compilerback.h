#include <iostream>
#include <fstream>
#include <cstring>
#include <unordered_map>

using namespace std;

#define CODE_LEN  50000
#define FILE_NAME 100



struct ins{
    int opcode;
    string arg1;
    string arg2; 
    string arg3; 
    string assembly;
};



void merge(int argc, char *argv[]);
void write(struct ins commandList[], int commandNum, char* dest);
void createHashTab();
void translate(struct ins commandList[], int commandNum);
void alu (struct ins *command); 
void push(struct ins *command); 
void pop (struct ins *command); 
void control (struct ins *command); 
void function(struct ins *command);
string getFile(string function);
string clean(string command);