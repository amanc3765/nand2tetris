#include "compilerback.h"


extern string currFile;


void push(struct ins *command){

    string temp = "";
    int opcode = command->opcode;
    int fixed = 0;

    switch(opcode){

        case 21: temp += "@ARG  \n";    //ARGUMENT
        break;

        case 22: temp += "@LCL  \n";    //LOCAL
        break;

        case 23:                        //STATIC
        break;

        case 24:                        //CONSTANT
        break;

        case 25: temp += "@THIS \n";    //THIS
        break;

        case 26: temp += "@THAT \n";    //THAT
        break;

        case 27: temp += "@3    \n";   //POINTER
        fixed = 1;
        break;

        case 28: temp += "@5    \n";    //TEMP
        fixed = 1;
        break;

    }


    if(opcode == 23){                   //STATIC
        temp += 
            "@" + currFile + ".STATIC_" + command->arg3 + "\n"
            "D=M   \n";
    }else if(opcode == 24){
        temp +=
            "@" + command->arg3 + "\n" 
            "D=A   \n";
    }else{

        if(fixed){
            temp+= "D=A   \n";          //POINTER & TEMP
        }else{
            temp+= "D=M   \n";
        }  

        temp+=
            "@" + command->arg3 + "\n"
            "A=D+A \n"
            "D=M   \n";
    }

    temp+= 
        "@SP   \n"
        "A=M   \n"    
        "M=D   \n"

        "@SP   \n"
        "M=M+1 \n";
    

    command->assembly = temp;
}


void pop(struct ins *command){

    string temp = "";
    int opcode = command->opcode;
    int fixed = 0;

    switch(opcode){

        case 31: temp += "@ARG  \n";    //ARGUMENT
        break;

        case 32: temp += "@LCL  \n";    //LOCAL
        break;

        case 33:                        //STATIC
        break;

        case 34:                        //CONSTANT
        break;

        case 35: temp += "@THIS \n";    //THIS
        break;

        case 36: temp += "@THAT \n";    //THAT
        break;

        case 37: temp += "@3    \n";    //POINTER
        fixed = 1;
        break;

        case 38: temp += "@5    \n";    //TEMP
        fixed = 1;
        break;

    }

    if(opcode == 33){
        temp += 
            "@SP   \n"
            "AM=M-1\n"
            "D=M   \n"

            "@" + currFile + ".STATIC_" + command->arg3 + "\n"            
            "M=D   \n";

            command->assembly = temp;            
            return;

    }else if(fixed){            ///POINTER & TEMP
        temp+= "D=A   \n";
    }else{
        temp+= "D=M   \n";
    }   

    temp+=
        "@" + command->arg3 + "\n"
        "D=D+A \n"
        
        "@R13  \n"
        "M=D   \n"
        
        "@SP   \n"
        "AM=M-1\n"
        "D=M   \n"

        "@R13  \n"
        "A=M   \n"
        
        "M=D   \n";

    command->assembly = temp;

}