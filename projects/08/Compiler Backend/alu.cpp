#include "compilerback.h"


int labelNum = 0;


void alu(struct ins *command){

    string temp = "";
    int opcode = command->opcode;

    if(opcode <= 14){
        
        temp+= 
            "@SP   \n" 
            "M=M-1 \n"
            "A=M   \n"
            "D=M   \n"

            "@SP   \n"
            "M=M-1 \n"
            "A=M   \n";

        switch (opcode){
            case 11: temp += "D=D+M \n";    //x+y
            break;

            case 12: temp += "D=M-D \n";    //x-y
            break;

            case 13: temp += "D=D&M \n";    //x&y
            break;

            case 14: temp += "D=D|M \n";    //x|y
            break;
        }

        temp+=
            "M=D   \n"
            "@SP   \n"
            "M=M+1 \n";
        
    }else if(opcode <= 17){

        string label = to_string(labelNum++);

        temp+=
            "@SP   \n"
            "M=M-1 \n"
            "A=M   \n"
            "D=M   \n"

            "@SP   \n"
            "M=M-1 \n"
            "A=M   \n"
            "D=M-D \n"
            "@" + label + "_TRUE  \n";

        switch (opcode){
            case 15: temp += "D;JEQ \n";    //x==y
            break;

            case 16: temp += "D;JGT \n";    //x>y
            break;

            case 17: temp += "D;JLT \n";    //x<y
            break;
        }
        
        temp+=
            "@SP    \n"
            "A=M    \n"
            "M=0    \n"

            "@" + label + "_END   \n"
            "0;JMP  \n"

            "(" + label + "_TRUE) \n"
            "@SP    \n"
            "A=M    \n"
            "M=-1   \n"

            "(" + label + "_END)  \n"
            "@SP    \n"
            "M=M+1  \n";

    }else{

        temp+=
            "@SP   \n"
            "A=M-1 \n";

        switch (opcode){
            case 18: temp += "M=-M \n";     //-y
            break;

            case 19: temp += "M=!M \n";     //-y
            break;
        }
    }

    command->assembly = temp;

}
