#include "SyntaxAnalysis.h"



extern struct token tokenList[NUM_TOKENS];
extern int tokenNum, Index;

extern int indent;
extern fstream out;
extern string result;



/*
expression:         term (op term)*

term:               integerConstant | stringConstant | keywordConstant |
                    varName | varName '[' expression ']' | subroutineCall |
                    '(' expression ')' | unaryOp term

subroutineCall:     subroutineName '(' expressionList ')' | 
                    (className | varName) '.' subroutineName '(' expressionList ')'

expressionList:     (expression (',' expression)* )?

op:                 '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='

unaryOp:            '-' | '~'

KeywordConstant:    'true' | 'false' | 'null' | 'this'
*/



//expression: term (op term)*
bool parseExpression(){

    makeIndent(); result += "<expression>\n";  
    ++indent;


    //term
    if(!parseTerm()){
        return false;
    }

    //(op term)*
    while(tokenList[Index].code >= 210 && tokenList[Index].code <= 218){

        //op
        if(!parseOp()){
            return false;
        }

        //term
        if(!parseTerm()){
            return false;
        }

    }


    --indent;
    makeIndent(); result += "</expression>\n";

    return true;
}



// term: integerConstant | stringConstant | keywordConstant | 
//       varName | varName '[' expression ']' | subroutineCall | '(' expression ')' | unaryOp term 
bool parseTerm(){

    makeIndent(); result += "<term>\n";  
    ++indent;


    //varName | varName '[' expression ']' | subroutineName
    if(tokenList[Index].code == 500){

        //varName
        //subroutineName | (className | varName) 
        if(!match(500)){               
            cout<<"SE: term: varName | subroutineName\n";
            return false;
        }

        //'[' expression ']'
        if(tokenList[Index].code == 205){
            
            //'['
            if(!match(205)){               
                cout<<"SE: term: '['\n";
                return false;
            }
            
            //expression
            if(!parseExpression()){
                return false;
            }
            
            //']'
            if(!match(206)){               
                cout<<"SE: term: ']'\n";
                return false;
            }
        }

        //'(' expressionList ')' |
        //'.' subroutineName '(' expressionList ')'
        if(tokenList[Index].code == 203 || tokenList[Index].code == 207){
        
            //'(' expressionList ')'
            //'.' subroutineName '(' expressionList ')'
            if(!parseSubroutineCall()){
                return false;
            }
            
        }

    }

    //'(' expression ')'
    else if(tokenList[Index].code == 203){
            
        //'('
        if(!match(203)){               
            cout<<"SE: term: '('\n";
            return false;
        }
        
        //expression
        if(!parseExpression()){
            return false;
        }
        
        //')'
        if(!match(204)){               
            cout<<"SE: term: ')'\n";
            return false;
        }
    }

    //unaryOp term 
    else if(tokenList[Index].code == 211 || tokenList[Index].code == 219){

        //unaryOp: '-' | '~'
        if(!match(211) && !match(219)){               
            cout<<"SE: term: '-' | '~'\n";
            return false;
        }

        //term
        if(!parseTerm()){
            return false;
        }
        
    }
    
    //integerConstant | stringConstant | keywordConstant | 
    else if(
        !match(300) &&                                           // integerConstant  
        !match(400) &&                                           // stringConstant
        !match(112) && !match(113) && !match(114) && !match(115) // KeywordConstant: 'true' | 'false' | 'null' | 'this'
    ){  
        cout<<"SE: term: integerConstant | stringConstant | keywordConstant\n";
        return false;    
    }


    --indent;
    makeIndent(); result += "</term>\n";

    return true;

}



//op: '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
bool parseOp(){

    if(
        !match(210) && !match(211) && !match(212) && !match(213) &&
        !match(214) && !match(215) && !match(216) && !match(217) && !match(218) 
    ){               
        cout<<"SE: op: '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='\n";
        return false;
    }

    return true;
}



// subroutineCall: subroutineName '(' expressionList ')' | 
//                 (className | varName) '.' subroutineName '(' expressionList ')'
bool parseSubroutineCall(){
    
    //'.' subroutineName 
    if(tokenList[Index].code == 207){

        //'.'
        if(!match(207)){               
            cout<<"SE: subroutineCall: '.'\n";
            return false;
        }

        //subroutineName
        if(!match(500)){               
            cout<<"SE: subroutineCall: subroutineName\n";
            return false;
        }

    }

    //'('
    if(!match(203)){               
        cout<<"SE: subroutineCall: '('\n";
        return false;
    }
    
    //expressionList
    if(!parseExpressionList()){
        return false;
    }
    
    //')'
    if(!match(204)){               
        cout<<"SE: subroutineCall: ')'\n";
        return false;
    }

    return true;
}



//expressionList: (expression (',' expression)* )?
bool parseExpressionList(){

    makeIndent(); result += "<expressionList>\n";  
    ++indent;


    //()
    if(tokenList[Index].code == 204){
        goto expressionListLabel;
    }else{

        //expression
        if(!parseExpression()){
            return false;
        }

        //(',' expression)*
        while(tokenList[Index].code == 208){
            
            //','
            if(!match(208)){               
                cout<<"SE: expressionList: ','\n";
                return false;
            }

            //expression
            if(!parseExpression()){
                return false;
            }

        }
    }

    expressionListLabel:
    --indent;
    makeIndent(); result += "</expressionList>\n";

    return true;
}


