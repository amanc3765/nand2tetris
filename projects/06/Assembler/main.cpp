#include "assemblerback.h"


//Driver function main()
int main(int arc, char *argv[]){

    ifstream fin;    
    string dirtyBuffer, cleanBuffer;
    string commandList[CODE_LEN], labelList[LABEL_LEN];
    int commandNum, labelNum, i;
    int labelAdd[LABEL_LEN];

    fin.open(argv[1]);
    
    for(i= commandNum = labelNum = 0; fin; ++i){

        getline(fin,dirtyBuffer);
        cout<<dirtyBuffer<<endl;
        
        cleanBuffer = clean(dirtyBuffer);

        if(cleanBuffer.length() > 0){

            if(cleanBuffer[0] == '('){
                labelAdd[labelNum] = commandNum;
                labelList[labelNum] = cleanBuffer.substr(1);
                ++labelNum;
            }else{
                commandList[commandNum] = cleanBuffer;
                ++commandNum;                
            }
        } 
    }
    
    /*    
    //Print list of commands
    {
        cout<<endl;
        for(i=0; i<commandNum; ++i){
            cout<<i<<" : "<<commandList[i]<<endl;
        }
    }

    //Print list of labels
    {
        cout<<endl;
        for(i=0; i<labelNum; ++i){        
            cout<<labelAdd[i]<<" : "<<labelList[i]<<endl;
        }
    }
    */
    
    createSymbolTab(labelList, labelAdd, labelNum, commandList, commandNum);
    createHashTab();
    
    parse(commandList, commandNum);    
    assemble(commandList, commandNum);

    compare(argv[2]);

    return 0;

}


//Clean assembly line code
string clean(string dirtyBuffer){
    
    string cleanBuffer = "";  
    char curr;
    int i=0, len=0;

    while(1){
        curr = dirtyBuffer[i++];

        if(curr == '\n' || curr == '\0'){
            break;
        }else if(curr == ' ' || curr == '\t' || curr == ')'){
            continue;
        }else if(curr == '/'){
            if(i-1 >= 0 && dirtyBuffer[i-1] == '/'){
                break;
            }
        }else{
            cleanBuffer += curr;
        }  
    }

    return cleanBuffer;
}


//Assemble instruction code into file
void assemble(string commandList[], int commandNum){
    
    ofstream fout;
    fout.open("Result.hack");

    for(int i=0; i<commandNum; ++i){
        fout<<commandList[i]<<endl;
    }

}


//Match with output
int compare(char *file1){
    ifstream f1,f2;

    f1.open(file1);
    f2.open("Result.hack");

    string temp1,temp2;
    int i, line = 0;
    
    while(f1 && f2){
        getline(f1,temp1);
        getline(f2,temp2);

        cout<<temp1<<"\t"<<temp2<<endl;
        
        for(i=0; temp1[i]; ++i){
            if(temp1[i] != temp2[i]){
                cout<<"\nNot Equal\n";
                return line;
            }
        }

        ++line;
    }

    cout<<"\nEqual\n";
    return -1;
}