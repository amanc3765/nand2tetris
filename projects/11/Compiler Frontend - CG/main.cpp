#include "SyntaxAnalysis.h"
// g++ main.cpp token.cpp class.cpp statements.cpp expression.cpp symbol.cpp



extern struct token tokenList[NUM_TOKENS];
extern int tokenNum, tokenIndex;

extern bool endLine;
extern string code_XML, code_VM;



int main(int argc, char* argv[]){

    createHashTab();
    tokenize(argv[1]);       //.Jack class file



    ofstream out;
    out.open("Tokens.xml");

    if(out){
        cout<<"Tokens.xml open successfull. \n\n";
    }else{
        cout<<"Tokens.xml open failed. \n\n";
    }

    out<<"<tokens>\n";
    for(int i=0; i<tokenNum; ++i){
        //cout<<"<"<<tokenList[i].tag<<"> "<<tokenList[i].val<<" </"<<tokenList[i].tag<<">\n";
        out<<"<"<<tokenList[i].tag<<"> "<<tokenList[i].val<<" </"<<tokenList[i].tag<<">\n";
    }
    out<<"</tokens>\n";
    
    out.close();
    


    out.open("Parse.xml");

    if(out){
        cout<<"Parse.xml open successfull. \n\n";
    }else{
        cout<<"Parse.xml open failed. \n\n";
    }

    tokenIndex = 0;

    //class
    if(parse_Class()){
        out<<code_XML;
        cout<<"\n\nParse Successfull.\n";
    }else{
        cout<<"\n\nParse Failed.\n";
    }

    out.close();
    


    out.open("Code.vm");

    if(out){
        cout<<"Code.vm open successfull. \n\n";
    }else{
        cout<<"Code.vm open failed. \n\n";
    }

    out<<code_VM;
    cout<<code_VM<<endl;

    return 0;
}



void tokenize(char* file){

    ifstream in;
    string buffer, token;


    in.open(file);      //.Jack class file
    cout<<endl;

    if(file){
        cout<<file<<" open successfull. \n\n";
    }else{
        cout<<file<<" open failed. \n\n";
    }


    for(int i=1; in; ++i){
        
        getline(in,buffer);
        //cout<<endl<<i<<" "<<buffer<<endl;

        tokenIndex = 0;
        endLine = false;

        while(!endLine){

            token = getToken(buffer);
        
            if(token.length() > 0){
                classifyToken(token);
            }
        }         
    }
}
