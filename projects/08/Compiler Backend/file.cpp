#include "compilerback.h"


//Merge VM files
void merge(int argc, char *argv[]){

    ifstream in;
    ofstream out;
    string dirtyBuffer, cleanBuffer;
    char* src = new char[FILE_NAME];
    int line = 0;


    //Temp.txt store result of merge of all VM files
    out.open("Temp.txt");    
    if(out){
        cout<<"\nTemp.txt open successful. \n\n";
    }else{
        cout<<"\nTemp.txt open failed. \n\n";
    }  


    for(int i=2; i<argc; ++i){
        
        for(int j=0; j<FILE_NAME; ++j){
            src[j] = '\0';
        }    
        strcat(src,argv[i]);
        strcat(src,".vm");    
    

        //Open VM files
        in.open(src);
        cout<<endl;
        if(in){
            cout<<src<<" open successful. \n\n";
        }else{
            cout<<src<<" open failed. \n\n";
        }


        while(in){
            getline(in,dirtyBuffer);
            cout<<line++<<"__"<<dirtyBuffer<<endl;

            cleanBuffer = clean(dirtyBuffer);
            if(cleanBuffer.length() > 0){
                out<<cleanBuffer<<endl;
            }            
        }

        in.close();
    }

    out.close();
}


//Clean VM code
string clean(string dirtyBuffer){

    string cleanBuffer = "";  
    char curr;

    for(int i=0; dirtyBuffer[i]; ++i){
        curr = dirtyBuffer[i];

        if(curr == '\n'){
            break;
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


//Write assembly code to file
void write(struct ins commandList[], int commandNum, char* file){

    ofstream out;
    out.open(file);
    
    cout<<endl;
    if(out){
        cout<<file<<" open successful. \n\n";
    }else{
        cout<<file<<" open failed. \n\n";
    }

    for(int i=0; i<commandNum; ++i){
        out<<commandList[i].assembly;
    }
    
    out.close();



    ifstream in;
    in.open(file);

    if(in){
        cout<<file<<" open successful. \n\n";
    }else{
        cout<<file<<" open failed. \n\n";
    }

    // string temp;
    // for(int i=0; in; ++i){
    //     getline(in,temp);
    //     cout<<i<<" : "<<temp<<endl;
    // }
    
}