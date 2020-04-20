#include "compilerback.h"


struct ins commandList[CODE_LEN];


int main(int argc, char *argv[]){

    merge(argc, argv);

    ifstream fin;    
    string buffer; 
    int commandNum, i;


    fin.open("Temp.txt");
    if(fin){
        cout<<"\nTemp.txt open successful. \n\n";
    }else{
        cout<<"\nTemp.txt open failed. \n\n";
    }  


    //Break lines into components
    for(i= commandNum= 0; fin; ++i){
        
        getline(fin,buffer);

        if(buffer.length() > 0){

            struct ins command;
            string temp;
            int j, count;

            command.arg1= command.arg2= command.arg3= temp= "";
            j= count= 0;

            while(1){

                if(buffer[j] == ' ' || buffer[j] == '\0'){
                    ++count;

                    switch(count){
                        case 1: 
                            command.arg1 = temp;
                            temp = "";
                        break;

                        case 2: 
                            command.arg2 = temp;
                            temp = "";
                        break;

                        case 3: 
                            command.arg3 = temp;
                            temp = "";
                        break;
                    }

                }else{
                    temp += buffer[j];
                }

                if(buffer[j] == '\0'){
                    break;
                }

                ++j;
            }

            commandList[commandNum++] = command; 
        }
    }


    //Translate VM code to Assembly code
    translate(commandList,commandNum);


    //Print list of VM commands
    cout<<endl;
    for(i=0; i<commandNum; ++i){

        struct  ins *curr = &commandList[i];
        
        cout<<curr->opcode   <<" |_"
            <<curr->arg1     <<"_|_"
            <<curr->arg2     <<"_|_"
            <<curr->arg3     <<"_|\n"
            <<curr->assembly <<endl;
    }


    //Write Assembly code to output file
    write(commandList,commandNum,argv[1]);


    return 0;
}




