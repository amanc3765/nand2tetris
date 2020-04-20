#include "compilerback.h"


unordered_map<string,int> command;


//Detect opcode of commands
void translate(struct ins commandList[], int commandNum){
    
    createHashTab();

    for(int i= 0; i<commandNum; ++i){

        struct  ins *curr = &commandList[i];
        curr->opcode = command[curr->arg1];

        if(curr->opcode %10 == 0){
            curr->opcode += command[curr->arg2];
        }

        if(curr->opcode >=51){
            function(curr);
        }else if(curr->opcode >= 41){
            control(curr);
        }else if(curr->opcode >= 31){
            pop(curr);
        }else if(curr->opcode >= 21){
            push(curr);
        }else{
            alu(curr);
        }
    }
}


void createHashTab(){

    command["add"] = 11;
    command["sub"] = 12;
    command["and"] = 13;
    command["or"]  = 14;     
    command["eq"]  = 15;
    command["gt"]  = 16;
    command["lt"]  = 17;
    command["neg"] = 18;
    command["not"] = 19;

    command["argument"] = 1;
    command["local"]    = 2;
    command["static"]   = 3;
    command["constant"] = 4;     
    command["this"]     = 5;
    command["that"]     = 6;
    command["pointer"]  = 7;
    command["temp"]     = 8;

    command["push"] = 20;
    command["pop"]  = 30;

    command["label"]   = 41;
    command["goto"]    = 42;
    command["if-goto"] = 43;

    command["call"]     = 51;
    command["function"] = 52;    
    command["return"]   = 53;

}


