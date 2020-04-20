#include "SyntaxAnalysis.h"



struct token tokenList[NUM_TOKENS];
int tokenNum, Index;

int comment;
bool endLine;

unordered_map <string,int> keyword;
unordered_map <string,int> symbol;



string getToken(string line){

    char curr;
    string token = "";
    
    while(line[Index]){

        curr = line[Index];        
        
        if(comment){        

            if(curr == '*' && line[Index+1] == '/'){
                comment = 0;
                Index += 2;
                continue;
            }

        }else if(curr == '\"'){   

            //String Constant
            while(line[++Index] != '\"'){
                token += line[Index];
            }
            ++Index;
            return "\"" + token;

        }else if(isIdentifier(curr)){

            //Identifier
            while(isIdentifier(line[Index])){
                token += line[Index++];            
            }
            return token;

        }else if(curr == ' ' || curr == '\t'){
        
            //White Spaces
            while(line[Index] == ' ' || line[Index] == '\t') {
                ++Index;
            }  
            return  token;

        }else if(curr == '/'){    

            //Comments
            if(line[Index+1] == '/'){
                endLine = true;
                return token;
            }else if(line[Index+1] == '*'){
                comment = 1;
                Index += 2;
                continue;
            }else{
                token += line[Index++]; // '/'
                return token;
            }

        }else{

            //Symbols
            token += line[Index++];
            return token;
        }
        
        ++Index;
    }

    endLine = true;
    return token;
}



bool isIdentifier(char c){
    if(
        (c >= 'a' && c <= 'z') || 
        (c >= 'A' && c <= 'Z') || 
        (c >= '0' && c <= '9') || 
        (c == '_')
    ) {
        return true;
    }

    return false;
}



void classifyToken(string token){
    
    if(token.length() == 1){
        //Symbol
        auto temp = symbol.find(token);

        if(temp != symbol.end()){
            addToken(200+temp->second,"symbol",token);
            return;
        }

    }
    

    if('0' <= token[0] && token[0] <= '9'){
        //Integer Constant
        int isInteger, i;

        for(i= isInteger = 1; token[i]; ++i){
            if(token[i] < '0' || token[i] > '9'){
                isInteger = 0;
                cout<<"Lexical Error: Invalid integer constant.\n";
            }
        }

        addToken(300,"integerConstant",token);
        return;

    }else if(token[0] == '\"'){
        //String Constant
        addToken(400,"stringConstant",token.substr(1));
        return;

    } 

    auto temp = keyword.find(token);

    if(temp == keyword.end()){
        addToken(500,"identifier", token);          //Identifier        
    }else{
        addToken(100+temp->second,"keyword",token); //Keyword
    }

}



void addToken(int code, string tag, string val){
    
    struct token newToken;

    newToken.code = code;
    newToken.tag = tag;    

    if(code == 214){
        val = "&amp;";
    }else if(code == 216){
        val = "&lt;";
    }else if(code == 217){
        val = "&gt;";
    } 

    newToken.val = val;

    tokenList[tokenNum++] = newToken;

}



void createHashTab(){

    keyword["class"]        = 1;
    keyword["constructor"]  = 2;
    keyword["function"]     = 3;
    keyword["method"]       = 4;
    keyword["field"]        = 5;
    keyword["static"]       = 6;
    keyword["var"]          = 7;
    keyword["int"]          = 8;
    keyword["char"]         = 9;
    keyword["boolean"]      = 10;
    keyword["void"]         = 11;
    keyword["true"]         = 12;
    keyword["false"]        = 13;
    keyword["null"]         = 14;
    keyword["this"]         = 15;
    keyword["let"]          = 16;
    keyword["do"]           = 17;
    keyword["if"]           = 18;
    keyword["else"]         = 19;
    keyword["while"]        = 20;
    keyword["return"]       = 21;


    symbol["{"] = 1;
    symbol["}"] = 2;
    symbol["("] = 3;
    symbol[")"] = 4;
    symbol["["] = 5;
    symbol["]"] = 6;
    symbol["."] = 7;
    symbol[","] = 8;
    symbol[";"] = 9;
    symbol["+"] = 10;
    symbol["-"] = 11;
    symbol["*"] = 12;
    symbol["/"] = 13;
    symbol["&"] = 14;
    symbol["|"] = 15;
    symbol["<"] = 16;
    symbol[">"] = 17;
    symbol["="] = 18;
    symbol["~"] = 19;

}