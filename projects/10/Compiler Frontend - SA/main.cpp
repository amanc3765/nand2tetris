#include "SyntaxAnalysis.h"
//g++ -g main.cpp token.cpp class.cpp statements.cpp expression.cpp



extern struct token tokenList[NUM_TOKENS];
extern int tokenNum, Index;

extern bool endLine;
extern string result;



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
    cout<<tokenNum<<endl;
    
    out.close();
    

    out.open("Parse.xml");

    if(out){
        cout<<"Parse.xml open successfull. \n\n";
    }else{
        cout<<"Parse.xml open failed. \n\n";
    }

    Index = 0;

    //class
    if(!parseClass()){
        return false;
    }else{
        cout<<"Parse Successfull.\n";
        cout<<result<<endl;
        out<<result;
    }


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

        Index = 0;
        endLine = false;

        while(!endLine){

            token = getToken(buffer);
        
            if(token.length() > 0){
                //cout<<token<<"_";
                classifyToken(token);
            }
        }     
    
    }

}
