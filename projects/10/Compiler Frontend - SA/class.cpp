#include "SyntaxAnalysis.h"



extern struct token tokenList[NUM_TOKENS];
extern int tokenNum, Index;

int indent;
ofstream out;
string result;



bool match(int tokenCode){
    
    if(tokenList[Index].code == tokenCode){
        //cout<<"<"<<tokenList[Index].tag<<"> "<<tokenList[Index].val<<" </"<<tokenList[Index].tag<<">\n";
        makeIndent(); result += "<" + tokenList[Index].tag + "> " + tokenList[Index].val + " </" + tokenList[Index].tag + ">\n";
        ++Index;

        return true;
    }

    return false;
}



void makeIndent(){

    for(int i=0; i<indent; ++i){
        result += "  ";
    }

}



/*
class: 'class'  className '{' classVarDec* subroutineDec* '}'

classVarDec:    ('static' | 'field') type varName (',' varName)* ';'

type:           'int' | 'char' | 'boolean' | className

subroutineDec:  ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody

parameterList:  ((type varName) (',' type varName)*)?

subroutineBody: '{' varDec* statements '}'

varDec:         'var' type varName (',' varName)* ';'

className:      identifier

subroutineName: identifier

varName:        identifier
*/



//class: 'class'  className '{' classVarDec* subroutineDec* '}'
bool parseClass(){

    makeIndent(); result += "<class>\n";  
    ++indent;

    
    //'class'
    if(!match(101)){               
        cout<<"SE: Class: 'class'\n";
        return false;
    }

    //className
    if(!match(500)){               
        cout<<"SE: Class: className\n";
        return false;
    }

    //'{' 
    if(!match(201)){               
        cout<<"SE: Class: '{'\n";
        return false;
    }


    //classVarDec*
    while(tokenList[Index].code == 105 || tokenList[Index].code == 106){

        //classVarDec
        if(!parseClassVarDec()){
            return false;
        }       
    }

    //subroutineDec*
    while(tokenList[Index].code >= 102 && tokenList[Index].code <= 104){

        //subroutineDec
        if(!parseSubroutineDec()){
            return false;
        }        
    }

    //'}'
    if(!match(202)){               
        cout<<"SE: Class: '}'\n";
        return false;
    }


    --indent;
    makeIndent(); result += "</class>\n";

    return true;
}



//classVarDec:    ('static' | 'field') type varName (',' varName)* ';'
bool parseClassVarDec(){

    makeIndent(); result += "<classVarDec>\n";  
    ++indent;   


    //('static' | 'field')
    if(!match(105) && !match(106)){               
        cout<<"SE: classVarDec: ('static' | 'field')\n";
        return false;
    }
    
    //type
    if(!parseType("classVarDec:")){
        return false;
    }

    //varName
    if(!match(500)){               
        cout<<"SE: classVarDec: varName\n";
        return false;
    }

    //(',' varName)*
    while(tokenList[Index].code == 208){

        //','
        if(!match(208)){               
            cout<<"SE: classVarDec: ','\n";
            return false;
        }

        //varName
        if(!match(500)){               
            cout<<"SE: classVarDec: varName\n";
            return false;
        }

    }

    //';'
    if(!match(209)){               
        cout<<"SE: classVarDec: ';'\n";
        return false;
    }


    --indent;
    makeIndent(); result += "</classVarDec>\n"; 

    return true;
}



//('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody
bool parseSubroutineDec(){

    makeIndent(); result += "<subroutineDec>\n";  
    ++indent;
    

    //('constructor' | 'function' | 'method')
    if(!match(102) && !match(103) && !match(104)){               
        cout<<"SE: subroutineDec: ('constructor' | 'function' | 'method')\n";
        return false;
    }

    //('void' | type)
    if(!match(111) && !parseType("subroutineDec:")){
        cout<<"SE: subroutineDec: ('void' | type)\n";
        return false;
    }

    //subroutineName
    if(!match(500)){               
        cout<<"SE: subroutineDec: subroutineName\n";
        return false;
    }

    //'('
    if(!match(203)){               
        cout<<"SE: subroutineDec: '('\n";
        return false;
    }
    
    //parameterList
    if(!parseParameterList()){
        return false;
    }  

    //')'
    if(!match(204)){               
        cout<<"SE: subroutineDec: ')'\n";
        return false;
    }
    
    //subroutineBody
    if(!parseSubroutineBody()){
        return false;
    }        
   

    --indent;
    makeIndent(); result += "</subroutineDec>\n";        

    return true;
}



//'int' | 'char' | 'boolean' | className
bool parseType(string func){

    if(!match(108) && !match(109) && !match(110) && !match(500)){               
        cout<<"SE: parseType " + func + "type\n";
        return false;
    }

    return true;
}



//((type varName) (',' type varName)*)?
bool parseParameterList(){

    makeIndent(); result += "<parameterList>\n";
    ++indent; 


    //()
    if(tokenList[Index].code == 204){
        goto parameterListLabel;
    }else{
        
        //type 
        if(!parseType("parameterList:")){
            return false;
        }

        //varName
        if(!match(500)){               
            cout<<"SE: parameterList: varName)\n";
            return false;
        }

        //(',' type varName)*
        while(tokenList[Index].code == 208){

            //', 
            if(!match(208)){               
                cout<<"SE: parameterList: ','\n";
                return false;
            }

            //type
            if(!parseType("parameterList:")){
                return false;
            }

            //varName
            if(!match(500)){               
                cout<<"SE: parameterList: varName\n";
                return false;
            }

        }

    }

    parameterListLabel:
    --indent;
    makeIndent(); result += "</parameterList>\n";

    return true;    
}



//'{' varDec* statements '}'
bool parseSubroutineBody(){
    
    makeIndent(); result += "<subroutineBody>\n";
    ++indent;


    ///'{'
    if(!match(201)){               
        cout<<"SE: subroutineBody: '{'\n";
        return false;
    }

    //varDec*
    while(tokenList[Index].code == 107){

        //varDec
        if(!parseVarDec()){
            return false;
        }
    }

    //statements   
    if(!parseStatements()){
        return false;
    }
    
    //'}'
    if(!match(202)){               
        cout<<"SE: subroutineBody: '}'\n";
        return false;
    }


    --indent;
    makeIndent(); result += "</subroutineBody>\n";

    return true;
}



//'var' type varName (',' varName)* ';'
bool parseVarDec(){

    makeIndent(); result += "<varDec>\n";  
    ++indent; 


    //'var'
    if(!match(107)){               
        cout<<"SE: varDec: 'var'\n";
        return false;
    }

    //type
    if(!parseType("varDec:")){
        return false;
    }

    //varName
    if(!match(500)){               
        cout<<"SE: varDec: varName\n";
        return false;
    }

    //(',' varName)*
    while(tokenList[Index].code == 208){

        //','
        if(!match(208)){               
            cout<<"SE: varDec: ','\n";
            return false;
        }

        //varName
        if(!match(500)){               
            cout<<"SE: varDec: varName\n";
            return false;
        }

    }

    //';'
    if(!match(209)){               
        cout<<"SE: varDec: ';'\n";
        return false;
    }


    --indent;
    makeIndent(); result += "</varDec>\n";

    return true;
}


