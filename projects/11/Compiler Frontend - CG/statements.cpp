#include "SyntaxAnalysis.h"



extern struct token tokenList[NUM_TOKENS];
extern int tokenIndex;

extern string code_XML, code_VM;
extern int indent;

extern string name_Class;

extern string call_SubRoutine;
extern bool this_SubRoutine, object_SubRoutine;


extern int num_ActualParams;

int label_Loop, label_Condition;



/*
statements:     statement*

statement:      letStatement | ifStatement | whileStatement | doStatement | returnStatement

letStatement:   'let' varName ('[' expression ']')? '=' expression ';'

ifStatement:    'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?

whileStatement: 'while' '(' expression ')' '{' statements '}'

doStatement:    'do' call_SubRoutine ';'

ReturnStatement 'return' expression? ';'
*/



//statements: statement*
bool parse_Statements(){

    makeIndent(); code_XML += "<statements>\n";  
    ++indent;


    while(
        tokenList[tokenIndex].code >= KEY_let && 
        tokenList[tokenIndex].code <= KEY_return
    ){
        switch(tokenList[tokenIndex].code){
            case 116: parse_LetStatement();
            break;

            case 117: parse_DoStatement();
            break;

            case 118: parse_IfStatement();
            break;

            case 120: parse_WhileStatement();
            break;

            case 121: parse_ReturnStatement();
            break;
        }
    }


    --indent;
    makeIndent(); code_XML += "</statements>\n";

    return true;
}



//letStatement: 'let' varName ('[' expression ']')? '=' expression ';'
bool parse_LetStatement(){

    makeIndent(); code_XML += "<letStatement>\n";  
    ++indent;


    //'let'
    if(!match(KEY_let)){               
        cout<<"SE: letStatement: 'let'\n";
        return false;
    }
    
    struct token *token_Let_LVALUE;
    //varName
    if(match(IDENTIFIER)){    
        token_Let_LVALUE = &(tokenList[tokenIndex-1]);
    }else{          
        cout<<"SE: letStatement: varName\n";
        return false;
    }

    bool var_Array = false;
    //('[' expression ']')?
    if(tokenList[tokenIndex].code == SYM_sqr_open){

        var_Array = true;   

        //'['
        if(!match(SYM_sqr_open)){               
            cout<<"SE: letStatement: '['\n";
            return false;
        }

        //expression
        if(!parse_Expression()){
            cout<<"SE: letStatement: expression\n"; //Index of Array element
            return false;
        }

        //']'
        if(!match(SYM_sqr_close)){               
            cout<<"SE: letStatement: ']'\n";
            return false;
        }

        //Address calculation of LVALUE        
        struct entry *ST_entry = ST_search(token_Let_LVALUE->val);
        if(ST_entry){
            code_VM += "push " + ST_entry->scope + " " + ST_entry->index + "\n"; //Base address of Array
            code_VM += "add\n";
        }
    }

    //'='
    if(!match(SYM_equal)){               
        cout<<"SE: letStatement: '='\n";
        return false;
    }

    //expression
    if(!parse_Expression()){
        cout<<"SE: letStatement: expression\n";
        return false;
    }

    //';'
    if(!match(SYM_semi_colon)){               
        cout<<"SE: letStatement: ';'\n";
        return false;
    }

    if(var_Array){

        //pop that index
        code_VM += "pop temp 0\n";
        code_VM += "pop pointer 1\n";
        code_VM += "push temp 0\n";
        code_VM += "pop that 0\n";
    }else{

        //pop segment tokenIndex
        struct entry *ST_entry = ST_search(token_Let_LVALUE->val);
        if(ST_entry){
            code_VM += "pop " + ST_entry->scope + " " + ST_entry->index + "\n";
        }else{
            cout<<"let LVALUE not found in ST\n";
        }    
    }


    --indent;
    makeIndent(); code_XML += "</letStatement>\n";

    return true;
}



//ifStatement: 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?
bool parse_IfStatement(){

    makeIndent(); code_XML += "<ifStatement>\n";  
    ++indent;


    //'if'
    if(!match(KEY_if)){               
        cout<<"SE: ifStatement: 'if'\n";
        return false;
    }

    //'('
    if(!match(SYM_par_open)){               
        cout<<"SE: ifStatement: '('\n";
        return false;
    }

    //expression
    if(!parse_Expression()){
        cout<<"SE: ifStatement: parse_Expression\n";
        return false;
    }

    //')'
    if(!match(SYM_par_close)){               
        cout<<"SE: ifStatement: ')'\n";
        return false;
    }

    string curr_label_Condition = to_string(label_Condition++);
    code_VM += "if-goto IF_TRUE" + curr_label_Condition + "\n";
    code_VM += "goto IF_FALSE" + curr_label_Condition + "\n";
    code_VM += "label IF_TRUE" + curr_label_Condition + "\n";

    //'{'
    if(!match(SYM_curly_open)){               
        cout<<"SE: ifStatement: '{'\n";
        return false;
    }

    //statements
    if(!parse_Statements()){
        cout<<"SE: ifStatement: parse_Statements\n";
        return false;
    }

    //'}'
    if(!match(SYM_curly_close)){               
        cout<<"SE: ifStatement: '}'\n";
        return false;
    }
    
    //('else' '{' statements '}')?    
    if(tokenList[tokenIndex].code == KEY_else){ 

        code_VM += "goto IF_END" + curr_label_Condition + "\n";
        code_VM += "label IF_FALSE" + curr_label_Condition + "\n";

        //'else'
        if(!match(KEY_else)){               
            cout<<"SE: elseStatement: 'else'\n";
            return false;
        }

        //'{'
        if(!match(SYM_curly_open)){               
            cout<<"SE: elseStatement: '{'\n";
            return false;
        }

        //statements
        if(!parse_Statements()){
            cout<<"SE: elseStatement: parse_Statements\n";
            return false;
        }

        //'}'
        if(!match(SYM_curly_close)){               
            cout<<"SE: elseStatement: '}'\n";
            return false;
        }

        code_VM += "label IF_END" + curr_label_Condition + "\n"; 
    }else{
        code_VM += "label IF_FALSE" + curr_label_Condition + "\n"; 
    }


    --indent;
    makeIndent(); code_XML += "</ifStatement>\n";

    return true;
}



//whileStatement: 'while' '(' expression ')' '{' statements '}'
bool parse_WhileStatement(){

    makeIndent(); code_XML += "<whileStatement>\n";  
    ++indent;


    //'while'
    if(!match(KEY_while)){               
        cout<<"SE: whileStatement: 'while'\n";
        return false;
    }

    string curr_label_Loop = to_string(label_Loop++);
    code_VM += "label WHILE_EXP"+ curr_label_Loop + "\n";

    //'('
    if(!match(SYM_par_open)){               
        cout<<"SE: whileStatement: '('\n";
        return false;
    }

    //expression
    if(!parse_Expression()){
        cout<<"SE: whileStatement: parse_Expression\n";
        return false;
    }
    
    //')'
    if(!match(SYM_par_close)){               
        cout<<"SE: whileStatement: ')'\n";
        return false;
    }

    code_VM += "not\n";
    code_VM += "if-goto WHILE_END" + curr_label_Loop + "\n";

    //'{'
    if(!match(SYM_curly_open)){               
        cout<<"SE: whileStatement: '{'\n";
        return false;
    }

    //statements
    if(!parse_Statements()){
        cout<<"SE: whileStatement: parse_Statements\n";
        return false;
    }

    //'}'
    if(!match(SYM_curly_close)){               
        cout<<"SE: whileStatement: '}'\n";
        return false;
    }

    code_VM += "goto WHILE_EXP" + curr_label_Loop + "\n";
    code_VM += "label WHILE_END" + curr_label_Loop + "\n";


    --indent;
    makeIndent(); code_XML += "</whileStatement>\n";

    return true;
}



//doStatement: 'do' call_SubRoutine ';'
// call_SubRoutine:    subroutineName '(' expressionList ')' | 
//                     (className | varName) '.' subroutineName '(' expressionList ')'
bool parse_DoStatement(){

    makeIndent(); code_XML += "<doStatement>\n";  
    ++indent;


    //'do'
    if(!match(KEY_do)){               
        cout<<"SE: doStatement: 'do'\n";
        return false;
    }

    //subroutineName
    object_SubRoutine = false;    
    if(match(IDENTIFIER)){   

        struct entry *ST_entry = ST_search(tokenList[tokenIndex-1].val);
        if(ST_entry){
            //varName.subroutineName (expressionList)
            object_SubRoutine = true;
            call_SubRoutine += ST_entry->type;
            code_VM += "push " + ST_entry->scope + " " + ST_entry->index + "\n";  //Address of object
        }else{
            //subroutineName (expressionList) 
            //className.subroutineName (expressionList)
            call_SubRoutine += tokenList[tokenIndex-1].val; 
        }    
        
    }else{            
        cout<<"SE: doStatement: subroutineName\n";
        return false;
    }

    //call_SubRoutine
    if(parse_SubroutineCall()){

        if(object_SubRoutine){     
            //varName.subroutineName (expressionList)    
            code_VM += "call " + call_SubRoutine + " " + to_string(num_ActualParams+1) + "\n";
        }else if(this_SubRoutine){
            //subroutineName (expressionList) 
            code_VM += "call " + name_Class + "." + call_SubRoutine + " " + to_string(num_ActualParams+1) + "\n";
        }else{
            //className.subroutineName (expressionList)
            code_VM += "call " + call_SubRoutine + " " + to_string(num_ActualParams) + "\n";
        }
        call_SubRoutine = ""; 
    }else{
        cout<<"SE: doStatement: call_SubRoutine\n";
        return false;
    }

    //';'
    if(!match(SYM_semi_colon)){               
        cout<<"SE: doStatement: ';'\n";
        return false;
    }

    code_VM += "pop temp 0\n";


    --indent;
    makeIndent(); code_XML += "</doStatement>\n";

    return true;
}



//ReturnStatement: 'return' expression? ';'
bool parse_ReturnStatement(){

    makeIndent(); code_XML += "<returnStatement>\n";  
    ++indent;


    //'return'
    if(!match(KEY_return)){               
        cout<<"SE: ReturnStatement: 'return'\n";
        return false;
    }

    //expression?
    if(tokenList[tokenIndex].code == SYM_semi_colon){
        //return;
        code_VM += "push constant 0\n";       
    }else{
        //expression
        if(!parse_Expression()){
            return false;
        }
    }
    
    //';'
    if(!match(SYM_semi_colon)){               
        cout<<"SE: ReturnStatement: ';'\n";
        return false;
    }

    code_VM += "return\n";


    --indent;
    makeIndent(); code_XML += "</returnStatement>\n";

    return true;
}