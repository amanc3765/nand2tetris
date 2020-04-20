#include "SyntaxAnalysis.h"



extern struct token tokenList[NUM_TOKENS];
extern int tokenIndex;

extern string code_XML, code_VM;
extern int indent;

extern string name_Class;

string call_SubRoutine;
bool this_SubRoutine, object_SubRoutine;


int num_ActualParams;



/*
expression:         term (op term)*

term:               integerConstant | stringConstant | keywordConstant |
                    varName | varName '[' expression ']' | call_SubRoutine |
                    '(' expression ')' | unaryOp term

call_SubRoutine:    subroutineName '(' expressionList ')' | 
                    (className | varName) '.' subroutineName '(' expressionList ')'

expressionList:     (expression (',' expression)* )?

op:                 '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='

unaryOp:            '-' | '~'

KeywordConstant:    'true' | 'false' | 'null' | 'this'
*/



//expression: term (op term)*
bool parse_Expression(){

    makeIndent(); code_XML += "<expression>\n";  
    ++indent;


    //term
    if(!parse_Term()){
        cout<<"SE: expression: term\n";
        return false;
    }

    //(op term)*
    while(
        tokenList[tokenIndex].code >= SYM_add && 
        tokenList[tokenIndex].code <= SYM_equal
    ){

        struct token *token_Op = &(tokenList[tokenIndex]);

        //op
        if(!parse_Op()){
            cout<<"SE: expression: op\n";
            return false;
        }

        //term
        if(!parse_Term()){
            cout<<"SE: expression: term\n";
            return false;
        }

        switch(token_Op->code){            

            case SYM_add:          code_VM += "add\n";
            break;

            case SYM_sub:          code_VM += "sub\n";
            break;

            case SYM_mul:          code_VM += "call Math.multiply 2\n";
            break;

            case SYM_div:          code_VM += "call Math.divide 2\n";
            break;

            case SYM_and:          code_VM += "and\n";
            break;

            case SYM_or:           code_VM += "or\n";
            break;

            case SYM_less_than:    code_VM += "lt\n";
            break;

            case SYM_greater_than: code_VM += "gt\n";
            break;

            case SYM_equal:        code_VM += "eq\n";
            break;

        }
    }


    --indent;
    makeIndent(); code_XML += "</expression>\n";

    return true;
}



// term: integerConstant | stringConstant | keywordConstant | 
//       varName | varName '[' expression ']' | call_SubRoutine | '(' expression ')' | unaryOp term 
bool parse_Term(){

    makeIndent(); code_XML += "<term>\n";  
    ++indent;


    struct token *token_ArrayExp;
    bool var_array = false;
    bool object_SubRoutine = false;


    //varName | varName '[' expression ']' | subroutineName
    if(tokenList[tokenIndex].code == IDENTIFIER){

        //varName
        //subroutineName | (className | varName) 
        if(match(IDENTIFIER)){    

            if(tokenList[tokenIndex].code == SYM_sqr_open){

                //varName '[' expression ']'
                var_array = true;
                token_ArrayExp  = &(tokenList[tokenIndex-1]);                
            }else{

                struct entry *ST_entry = ST_search(tokenList[tokenIndex-1].val);
                if(ST_entry){                    
                    if(
                        ST_entry->type != "int" && 
                        ST_entry->type != "char" &&
                        ST_entry->type != "boolean" &&
                        tokenList[tokenIndex].code == SYM_dot
                    ){
                        //varName.subroutineName (expressionList)
                        object_SubRoutine = true;
                        call_SubRoutine += ST_entry->type;
                    }
                    code_VM += "push " + ST_entry->scope + " " + ST_entry->index + "\n";  //Address of object | varName
                                                     
                }else{
                    //subroutineName (expressionList) 
                    //className.subroutineName (expressionList)
                    call_SubRoutine += tokenList[tokenIndex-1].val;
                }        
            }
            
        }else{    
            cout<<"SE: term: varName | subroutineName\n";
            return false;
        }        

        //'[' expression ']'
        if(tokenList[tokenIndex].code == SYM_sqr_open){
            
            //'['
            if(!match(SYM_sqr_open)){               
                cout<<"SE: term: '['\n";
                return false;
            }
            
            //expression
            if(!parse_Expression()){
                cout<<"SE: term: '[' expression ']'\n"; //Index of Array element
                return false;
            }
            
            //']'
            if(!match(SYM_sqr_close)){               
                cout<<"SE: term: ']'\n";
                return false;
            }

            struct entry *ST_entry = ST_search(token_ArrayExp->val);
            if(ST_entry){
                //Address calculation of Array element
                code_VM += "push " + ST_entry->scope + " " + ST_entry->index + "\n"; //Base address of Array
                code_VM += "add\n";
                //Access Array element
                code_VM += "pop pointer 1\n";
                code_VM += "push that 0\n";
            }   
        }

        //subroutineName (expressionList) 
        //className.subroutineName (expressionList)
        if(
            tokenList[tokenIndex].code == SYM_par_open || 
            tokenList[tokenIndex].code == SYM_dot
        ){
        
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
                cout<<"SE: term: call_SubRoutine\n";
                return false;
            }
        }
    }

    //'(' expression ')'
    else if(tokenList[tokenIndex].code == 203){
            
        //'('
        if(!match(203)){               
            cout<<"SE: term: '('\n";
            return false;
        }
        
        //expression
        if(!parse_Expression()){
            return false;
        }
        
        //')'
        if(!match(204)){               
            cout<<"SE: term: ')'\n";
            return false;
        }
    }

    //unaryOp term 
    else if(
        tokenList[tokenIndex].code == SYM_sub || 
        tokenList[tokenIndex].code == SYM_not
    ){
        struct token *token_UnaryOp = &(tokenList[tokenIndex]);

        //unaryOp: '-' | '~'
        if(!match(SYM_sub) && !match(SYM_not)){               
            cout<<"SE: term: '-' | '~'\n";
            return false;
        }

        //term
        if(!parse_Term()){
            cout<<"SE: term: parse_Term\n";
            return false;
        }

        if(token_UnaryOp->code == SYM_sub){
            code_VM += "neg\n";
        }else{
            code_VM += "not\n";
        }                
    }
    
    //integerConstant | stringConstant | keywordConstant | 
    else{

        struct token *token_Const = &(tokenList[tokenIndex]);
        string const_String = token_Const->val;

        if(
            match(CONST_int) ||                                                         // integerConstant 
            match(CONST_string) ||                                                      // stringConstant
            match(KEY_true) || match(KEY_false) || match(KEY_null) || match(KEY_this)   // KeywordConstant: 'true' | 'false' | 'null' | 'this'
        ){
            switch(token_Const->code){            

                case CONST_int:          
                    code_VM += "push constant " + token_Const->val + "\n";
                break;

                case CONST_string:         
                    code_VM += "push constant " + to_string(const_String.length()) + "\n";
                    code_VM += "call String.new 1\n";

                    for(int i=0; const_String[i]; ++i){
                        code_VM += "push constant " + to_string((int)const_String[i]) + "\n";
                        code_VM += "call String.appendChar 2\n";
                    }
                break;

                case KEY_true:      
                    code_VM += "push constant 0\n";
                    code_VM += "not\n";
                break;

                case KEY_false:      
                    code_VM += "push constant 0\n"; 
                break;

                case KEY_null:      
                    code_VM += "push constant 0\n";     
                break;

                case KEY_this:  
                    code_VM += "push pointer 0\n";          
                break;

            }
        }else{  
            cout<<"SE: term: integerConstant | stringConstant | keywordConstant\n";
            return false;    
        }       
    }


    --indent;
    makeIndent(); code_XML += "</term>\n";

    return true;
}



//op: '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
bool parse_Op(){

    if(
        !match(SYM_add) && !match(SYM_sub) && !match(SYM_mul) && !match(SYM_div) &&
        !match(SYM_and) && !match(SYM_or) &&
        !match(SYM_less_than) && !match(SYM_greater_than) && !match(SYM_equal) 
    ){               
        cout<<"SE: op: '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='\n";
        return false;
    }

    return true;
}



// call_SubRoutine: subroutineName '(' expressionList ')' | 
//                 (className | varName) '.' subroutineName '(' expressionList ')'
bool parse_SubroutineCall(){

    this_SubRoutine = true;
    //'.' subroutineName 
    if(tokenList[tokenIndex].code == SYM_dot){

        this_SubRoutine = false;
        //'.'
        if(!match(SYM_dot)){               
            cout<<"SE: call_SubRoutine: '.'\n";
            return false;
        }

        //subroutineName
        if(match(IDENTIFIER)){       
            //className.subroutineName (expressionList)
            call_SubRoutine += "." + tokenList[tokenIndex-1].val;
        }else{        
            cout<<"SE: call_SubRoutine: subroutineName\n";
            return false;
        }
    }

    //'('
    if(!match(SYM_par_open)){               
        cout<<"SE: call_SubRoutine: '('\n";
        return false;
    }
    
    //Push this pointer
    if(this_SubRoutine){
        code_VM += "push pointer 0\n";
    }

    num_ActualParams = 0;
    //expressionList
    if(!parse_ExpressionList()){
        cout<<"SE: call_SubRoutine: expressionList\n";
        return false;
    }

    //')'
    if(!match(SYM_par_close)){               
        cout<<"SE: call_SubRoutine: ')'\n";
        return false;
    }

    return true;
}



//expressionList: (expression (',' expression)* )?
bool parse_ExpressionList(){

    makeIndent(); code_XML += "<expressionList>\n";  
    ++indent;


    //()
    if(tokenList[tokenIndex].code == SYM_par_close){
        goto label_ExpressionList;
    }else{

        //expression
        if(parse_Expression()){
            ++num_ActualParams;
        }else{
            cout<<"SE: expressionList: ','\n";
            return false;
        }

        //(',' expression)*
        while(tokenList[tokenIndex].code == SYM_comma){
            
            //','
            if(!match(SYM_comma)){               
                cout<<"SE: expressionList: ','\n";
                return false;
            }

            //expression
            if(parse_Expression()){
                ++num_ActualParams;
            }else{
                cout<<"SE: expressionList: ','\n";
                return false;
            }
        }
    }

    label_ExpressionList:


    --indent;
    makeIndent(); code_XML += "</expressionList>\n";

    return true;
}