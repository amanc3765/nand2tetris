#include "compilerback.h"


extern string currFunc; 


void control (struct ins *command){

    string temp, label;
    temp = "";
    label = currFunc + "$" + command->arg2;

    int opcode = command->opcode;

    switch(opcode){
        case 41:
            temp += "(" + label + ")\n";  
        break;

        case 42:
            temp +=
                "@" + label + "\n"
                "0;JMP \n";
        break;

        case 43:
            temp +=
                "@SP    \n"
                "M=M-1  \n"
                "@SP    \n"
                "A=M    \n"
                "D=M    \n"                
                "@" + label + "\n"
                "D;JNE  \n";
                
        break;
    }

    command->assembly = temp;
}