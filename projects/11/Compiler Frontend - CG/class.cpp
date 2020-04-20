#include "SyntaxAnalysis.h"



extern struct token tokenList[NUM_TOKENS];
extern int tokenIndex;

string code_XML, code_VM;
int indent;

string name_Class;

string name_SubRoutine;
int type_Subroutine;

string type_Var;
int scope_ClassVar;
int num_Field, num_FormalParams, num_Local;

extern int argIndex;


bool match(int tokenCode){
    
    if(tokenList[tokenIndex].code == tokenCode){
        //cout<<"<"<<tokenList[tokenIndex].tag<<"> "<<tokenList[tokenIndex].val<<" </"<<tokenList[tokenIndex].tag<<">\n";
        makeIndent(); code_XML += "<" + tokenList[tokenIndex].tag + "> " + tokenList[tokenIndex].val + " </" + tokenList[tokenIndex].tag + ">\n";
        ++tokenIndex;

        return true;
    }
    return false;
}



void makeIndent(){

    for(int i=0; i<indent; ++i){
        code_XML += "  ";
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



//class: 'class' className '{' classVarDec* subroutineDec* '}'
bool parse_Class(){

    makeIndent(); code_XML += "<class>\n";  
    ++indent;

    
    //'class'
    if(!match(KEY_class)){               
        cout<<"SE: Class: 'class'\n";
        return false;
    }

    //className
    if(match(IDENTIFIER)){               
        name_Class = tokenList[tokenIndex-1].val;
    }else{        
        cout<<"SE: Class: className\n";
        return false;
    }

    //'{' 
    if(!match(SYM_curly_open)){               
        cout<<"SE: Class: '{'\n";
        return false;
    }

    //classVarDec*
    while(
        tokenList[tokenIndex].code == KEY_static || 
        tokenList[tokenIndex].code == KEY_field
    ){
        //classVarDec
        if(!parse_ClassVarDec()){
            cout<<"SE: Class: classVarDec\n";
            return false;
        }       
    }

    //subroutineDec*
    while(
        tokenList[tokenIndex].code >= KEY_constructor && 
        tokenList[tokenIndex].code <= KEY_method
    ){
        ST_clear();

        //subroutineDec        
        if(!parse_SubroutineDec()){
            cout<<"SE: Class: subroutineDec\n";
            return false;
        }        
    }

    //'}'
    if(!match(SYM_curly_close)){               
        cout<<"SE: Class: '}'\n";
        return false;
    }


    --indent;
    makeIndent(); code_XML += "</class>\n";

    return true;
}



//classVarDec: ('static' | 'field') type varName (',' varName)* ';'
bool parse_ClassVarDec(){

    makeIndent(); code_XML += "<classVarDec>\n";  
    ++indent;   


    //('static' | 'field')
    if(match(KEY_static)){
        scope_ClassVar = STATIC;
    }else if(match(KEY_field)){
        scope_ClassVar = FIELD;
    }else{               
        cout<<"SE: classVarDec: ('static' | 'field')\n";
        return false;
    }
    
    //type
    if(!parse_Type("classVarDec:")){
        return false;
    }

    //varName
    if(match(IDENTIFIER)){ 
        if(scope_ClassVar == STATIC){
            ST_addEntry(tokenList[tokenIndex-1].val, type_Var, STATIC);
        }else if(scope_ClassVar == FIELD){
            ++num_Field;
            ST_addEntry(tokenList[tokenIndex-1].val, type_Var, FIELD);
        }         
    }else{
        cout<<"SE: classVarDec: varName\n";
        return false;
    }

    //(',' varName)*
    while(tokenList[tokenIndex].code == SYM_comma){

        //','
        if(!match(SYM_comma)){               
            cout<<"SE: classVarDec: ','\n";
            return false;
        }

        //varName
        if(match(IDENTIFIER)){     
            if(scope_ClassVar == STATIC){
                ST_addEntry(tokenList[tokenIndex-1].val, type_Var, STATIC);
            }else if(scope_ClassVar == FIELD){
                ++num_Field;
                ST_addEntry(tokenList[tokenIndex-1].val, type_Var, FIELD);
            }     
        }else{    
            cout<<"SE: classVarDec: varName\n";
            return false;
        }
    }

    //';'
    if(!match(SYM_semi_colon)){
        cout<<"SE: classVarDec: ';'\n";
        return false;
    }


    --indent;
    makeIndent(); code_XML += "</classVarDec>\n"; 

    return true;
}



//subroutineDec: ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody
bool parse_SubroutineDec(){

    makeIndent(); code_XML += "<subroutineDec>\n";  
    ++indent;
    

    //('constructor' | 'function' | 'method')
    if(match(KEY_constructor) || match(KEY_function) || match(KEY_method)){    
        type_Subroutine = tokenList[tokenIndex-1].code; 
    }else{           
        cout<<"SE: subroutineDec: ('constructor' | 'function' | 'method')\n";
        return false;
    }

    //('void' | type)
    if(!match(KEY_void) && !parse_Type("subroutineDec:")){
        cout<<"SE: subroutineDec: ('void' | type)\n";
        return false;
    }

    //subroutineName
    if(match(IDENTIFIER)){  
        name_SubRoutine = tokenList[tokenIndex-1].val;
        code_VM += "function " + name_Class + "." + name_SubRoutine + " " /*num_Local*/; 
    }else{            
        cout<<"SE: subroutineDec: subroutineName\n";
        return false;
    }

    //'('
    if(!match(SYM_par_open)){               
        cout<<"SE: subroutineDec: '('\n";
        return false;
    }
    
    //parameterList
    if(!parse_ParameterList()){
        cout<<"SE: subroutineDec: parameterList\n";
        return false;
    }  

    //')'
    if(!match(SYM_par_close)){               
        cout<<"SE: subroutineDec: ')'\n";
        return false;
    }
    
    //subroutineBody
    if(!parse_SubroutineBody()){
        cout<<"SE: subroutineDec: subroutineBody\n";
        return false;
    }        
   

    --indent;
    makeIndent(); code_XML += "</subroutineDec>\n";        

    return true;
}



//'int' | 'char' | 'boolean' | className
bool parse_Type(string callingFunction){

    if(match(KEY_int) || match(KEY_char) || match(KEY_boolean) || match(IDENTIFIER)){
        type_Var = tokenList[tokenIndex-1].val;
    }else{               
        cout<<"SE: parse_Type [" + callingFunction + "]: type\n";
        return false;
    }

    return true;
}



//((type varName) (',' type varName)*)?
bool parse_ParameterList(){

    makeIndent(); code_XML += "<parameterList>\n";
    ++indent; 


    //()
    if(tokenList[tokenIndex].code == SYM_par_close){
        goto Label_parameterList;
    }else{

        if(type_Subroutine == KEY_method){
            ++argIndex;
        } 
        
        //type 
        if(!parse_Type("parameterList:")){
            return false;
        }

        //varName
        if(match(IDENTIFIER)){    
            ++num_FormalParams;
            ST_addEntry(tokenList[tokenIndex-1].val, type_Var, ARG);
        }else{           
            cout<<"SE: parameterList: varName\n";
            return false;
        }

        //(',' type varName)*
        while(tokenList[tokenIndex].code == SYM_comma){

            //',' 
            if(!match(SYM_comma)){               
                cout<<"SE: parameterList: ','\n";
                return false;
            }

            //type
            if(!parse_Type("parameterList:")){
                return false;
            }

            //varName
            if(match(IDENTIFIER)){         
                ++num_FormalParams;
                ST_addEntry(tokenList[tokenIndex-1].val, type_Var, ARG);
            }else{      
                cout<<"SE: parameterList: varName\n";
                return false;  
            }
        }
    }

    Label_parameterList:


    --indent;
    makeIndent(); code_XML += "</parameterList>\n";

    return true;    
}



//'{' varDec* statements '}'
bool parse_SubroutineBody(){   
    
    makeIndent(); code_XML += "<subroutineBody>\n";
    ++indent;


    ///'{'
    if(!match(SYM_curly_open)){               
        cout<<"SE: subroutineBody: '{'\n";
        return false;
    }   

    //varDec*
    while(tokenList[tokenIndex].code == KEY_var){

        //varDec
        if(!parse_VarDec()){
            cout<<"SE: subroutineBody: varDec\n";
            return false;
        }
    }
    code_VM += to_string(num_Local) + "\n";

    if(type_Subroutine == KEY_constructor){
        code_VM += "push constant " + to_string(num_Field) + "\n";
        code_VM += "call Memory.alloc 1\n";
        code_VM += "pop pointer 0\n";
    }else if(type_Subroutine == KEY_method){
        code_VM += "push argument 0\n";
        code_VM += "pop pointer 0\n";
    } 

    //statements   
    if(!parse_Statements()){
        cout<<"SE: subroutineBody: statements\n";
        return false;
    }
    
    //'}'
    if(!match(SYM_curly_close)){               
        cout<<"SE: subroutineBody: '}'\n";
        return false;
    }


    --indent;
    makeIndent(); code_XML += "</subroutineBody>\n";

    return true;
}



//'var' type varName (',' varName)* ';'
bool parse_VarDec(){

    makeIndent(); code_XML += "<varDec>\n";  
    ++indent; 


    //'var'
    if(!match(KEY_var)){               
        cout<<"SE: varDec: 'var'\n";
        return false;
    }

    //type
    if(!parse_Type("varDec:")){
        return false;
    }

    //varName
    if(match(IDENTIFIER)){          
        ++num_Local;
        ST_addEntry(tokenList[tokenIndex-1].val, type_Var, LOCAL);
    }else{     
        cout<<"SE: varDec: varName\n";
        return false;
    }

    //(',' varName)*
    while(tokenList[tokenIndex].code == SYM_comma){

        //','
        if(!match(SYM_comma)){               
            cout<<"SE: varDec: ','\n";
            return false;
        }

        //varName
        if(match(IDENTIFIER)){          
            ++num_Local;
            ST_addEntry(tokenList[tokenIndex-1].val, type_Var, LOCAL);
        }else{     
            cout<<"SE: varDec: varName\n";
            return false;
        }
    }

    //';'
    if(!match(SYM_semi_colon)){               
        cout<<"SE: varDec: ';'\n";
        return false;
    }


    --indent;
    makeIndent(); code_XML += "</varDec>\n";

    return true;
}