#include "assemblerback.h"


//Break instruction into DEST COMP JUMP components
void parse(string commandList[], int commandNum){

    string temp;
    int i, j, k;
    char curr;

    for(i=0; i<commandNum; ++i){        
        
        //A Instruction
        if(commandList[i][0] == '@'){

            struct A_ins ins_A;
            ins_A.label = commandList[i].substr(1); 
            
            //cout<<i<<": @ | "<<ins_A.label<<" | "<<endl;
            commandList[i] = transInsA(ins_A);
            
            continue;
        }
        

        //C Instrucion
        struct C_ins ins_C; 
        ins_C.dest = ins_C.comp = ins_C.jump = temp = ""; 
        j=0, k=0;

        //0: DEST = COMP
        //1: COMP ; JUMP

        while(1){
            curr = commandList[i][j++];

            switch (curr){
                case '\0':
                    if(ins_C.type == 0){        
                        ins_C.comp = temp;
                    }else{
                        ins_C.jump = temp;
                    }
                break;

                case '=': 
                    ins_C.type = 0;                
                    ins_C.dest = temp;
                    temp = "";                
                break;

                case ';': 
                    ins_C.type = 1;                
                    ins_C.comp = temp;
                    temp = "";
                break;

                default:
                    temp += curr;
            }

            if(curr == '\0'){
                break;
            }
        }

        //cout<<i<<": "<<ins_C.type<<" | "<<ins_C.dest<<" | "<<ins_C.comp<<" | "<<ins_C.jump<<" | "<<endl;
        commandList[i] = transInsC(ins_C);     
    }
}


