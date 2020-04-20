#include "compilerback.h"


string currFile = "";    //Curr File
string currFunc = "";    //Curr Function
int currFuncCall;        //For unique call address


void function (struct ins *command){
    
    int opcode = command->opcode;    
    string temp, start, push, end, count, returnadd;
    temp= start= end= push= count= returnadd="";
    
    switch (opcode){
        case 51:   
            returnadd = currFunc + "$RETURN_ADDRESS_" + to_string(currFuncCall++);            
            
            temp+=
                // push return-address (Using the label declared below)
                "@" + returnadd + "\n"
                "D=A    \n"
                "@SP    \n"
                "A=M    \n"
                "M=D    \n"
                "@SP    \n"
                "M=M+1  \n"

                // push LCL     Save LCL of the calling function
                "@LCL   \n"
                "D=M    \n" 
                "@SP    \n"
                "A=M    \n"
                "M=D    \n"
                "@SP    \n"
                "M=M+1  \n"

                // push ARG     Save ARG of the calling function
                "@ARG   \n"
                "D=M    \n" 
                "@SP    \n"
                "A=M    \n"
                "M=D    \n"
                "@SP    \n"
                "M=M+1  \n"

                // push THIS    Save THIS of the calling function
                "@THIS  \n"
                "D=M    \n" 
                "@SP    \n"
                "A=M    \n"
                "M=D    \n"
                "@SP    \n"
                "M=M+1  \n"

                // push THAT    Save THAT of the calling function
                "@THAT  \n"
                "D=M    \n" 
                "@SP    \n"
                "A=M    \n"
                "M=D    \n"
                "@SP    \n"
                "M=M+1  \n"

                // LCL = SP     Reposition LCL
                "@SP    \n"
                "D=M    \n"
                "@LCL   \n"
                "M=D    \n"

                // ARG = SP-n-5 Reposition ARG (n ¼ number of args.)
                "@SP    \n"
                "D=M    \n"
                "@" + command->arg3 + "\n"
                "D=D-A  \n"
                "@5     \n"
                "D=D-A  \n"
                "@ARG   \n"
                "M=D    \n"

                // goto f       Transfer control
                "@" + command->arg2 + "\n"
                "0;JMP  \n"

                // (return-address)  Declare a label for the return-address
                "(" + returnadd + ")\n";

        break;

        case 52:
            currFuncCall = 0;
            currFunc = command->arg2;   
            currFile = getFile(command->arg2);         

            start = currFunc + "$LOCAL_START";
            end   = currFunc + "$LOCAL_END";
            push  = currFunc + "$LOCAL_PUSH";

            temp+= 
                "(" + currFunc + ")\n"

                "@" + command->arg3 + "\n"
                "D=A            \n"
                "@R14           \n"
                "M=D            \n"

                "(" + start + " ) \n"

                "@R14           \n"
                "D=M            \n"
                "@" + push + "  \n"
                "D;JGT          \n"
                "@" + end + "   \n"
                "0;JMP          \n"

                "(" + push + ") \n"
                "@SP            \n"
                "A=M            \n"    
                "M=0            \n"
                "@SP            \n"
                "M=M+1          \n"
                "@R14           \n"
                "M=M-1          \n"
                
                "@" + start + " \n"
                "0;JMP          \n"

                "(" + end + " ) \n";
        break;

        case 53: 
            temp+=

                //FRAME = LCL       // FRAME is a temporary variable
                "@LCL   \n"
                "D=M    \n"
                "@R13   \n"
                "M=D    \n"

                //RET = *(FRAME-5)  // Put the return-address in a temp. var.
                "@R13   \n"
                "D=M    \n"
                "@5     \n"
                "A=D-A  \n"
                "D=M    \n"
                "@R15   \n"
                "M=D    \n"

                //*ARG = pop()      // Reposition the return value for the caller
                "@SP    \n"
                "A=M-1  \n"
                "D=M    \n"
                "@ARG   \n"
                "A=M    \n"
                "M=D    \n"

                //SP = ARG+1        // Restore SP of the caller
                "D=A+1  \n"
                "@SP    \n"
                "M=D    \n"              

                //THAT = *(FRAME-1) // Restore THAT of the caller
                "@R13   \n"
                "M=M-1  \n"
                "A=M    \n"
                "D=M    \n"
                "@THAT  \n"
                "M=D    \n"

                //THIS = *(FRAME-2) // Restore THIS of the caller
                "@R13   \n"
                "M=M-1  \n"
                "A=M    \n"
                "D=M    \n"
                "@THIS  \n"
                "M=D    \n"

                //ARG = *(FRAME-3)  // Restore ARG of the caller
                "@R13   \n"
                "M=M-1  \n"
                "A=M    \n"
                "D=M    \n"
                "@ARG   \n"
                "M=D    \n"

                //LCL = *(FRAME-4)  // Restore LCL of the caller
                "@R13   \n"
                "M=M-1  \n"
                "A=M    \n"
                "D=M    \n"
                "@LCL   \n"
                "M=D    \n"

                //goto RET          // Goto return-address (in the caller’s code)
                "@R15   \n"
                "A=M    \n"
                "0;JMP  \n";

        break;    
    }

    command->assembly = temp;

}


string getFile(string function){

    string file = "";
    
    for(int i=0; function[i] != '.'; ++i){
        file += function[i];
    }

    return file;
}