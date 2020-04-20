#include "SyntaxAnalysis.h"



extern struct token tokenList[NUM_TOKENS];
extern int tokenNum, Index;

extern int indent;
extern fstream out;
extern string result;



/*
statements:     statement*

statement:      letStatement | ifStatement | whileStatement | doStatement | returnStatement

letStatement:   'let' varName ('[' expression ']')? '=' expression ';'

ifStatement:    'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?

whileStatement: 'while' '(' expression ')' '{' statements '}'

doStatement:    'do' subroutineCall ';'

ReturnStatement 'return' expression? ';'
*/



//statements: statement*
bool parseStatements(){

    makeIndent(); result += "<statements>\n";  
    ++indent;


    while(tokenList[Index].code >= 116 && tokenList[Index].code <= 121){

        switch(tokenList[Index].code){
            case 116: parseLetStatement();
            break;

            case 117: parseDoStatement();
            break;

            case 118: parseIfStatement();
            break;

            case 120: parseWhileStatement();
            break;

            case 121: parseReturnStatement();
            break;
        }
    }


    --indent;
    makeIndent(); result += "</statements>\n";

    return true;
}



//letStatement: 'let' varName ('[' expression ']')? '=' expression ';'
bool parseLetStatement(){

    makeIndent(); result += "<letStatement>\n";  
    ++indent;


    //'let'
    if(!match(116)){               
        cout<<"SE: letStatement: 'let'\n";
        return false;
    }

    //varName
    if(!match(500)){               
        cout<<"SE: letStatement: varName\n";
        return false;
    }

    //('[' expression ']')?
    if(tokenList[Index].code == 205){

        //'['
        if(!match(205)){               
            cout<<"SE: letStatement: '['\n";
            return false;
        }

        //expression
        if(!parseExpression()){
            return false;
        }

        //']'
        if(!match(206)){               
            cout<<"SE: letStatement: ']'\n";
            return false;
        }
    }

    //'='
    if(!match(218)){               
        cout<<"SE: letStatement: '='\n";
        return false;
    }

    //expression
    if(!parseExpression()){
        return false;
    }

    //';'
    if(!match(209)){               
        cout<<"SE: letStatement: ';'\n";
        return false;
    }


    --indent;
    makeIndent(); result += "</letStatement>\n";

    return true;

}



//ifStatement: 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?
bool parseIfStatement(){

    makeIndent(); result += "<ifStatement>\n";  
    ++indent;


    //'if'
    if(!match(118)){               
        cout<<"SE: ifStatement: 'if'\n";
        return false;
    }

    //'('
    if(!match(203)){               
        cout<<"SE: ifStatement: '('\n";
        return false;
    }

    //expression
    if(!parseExpression()){
        return false;
    }

    //')'
    if(!match(204)){               
        cout<<"SE: ifStatement: ')'\n";
        return false;
    }

    //'{'
    if(!match(201)){               
        cout<<"SE: ifStatement: '{'\n";
        return false;
    }

    //statements
    if(!parseStatements()){
        return false;
    }

    //'}'
    if(!match(202)){               
        cout<<"SE: ifStatement: '}'\n";
        return false;
    }

    //('else' '{' statements '}')?    
    if(tokenList[Index].code == 119){

        //'else'
        if(!match(119)){               
            cout<<"SE: ifStatement: 'else'\n";
            return false;
        }

        //'{'
        if(!match(201)){               
            cout<<"SE: ifStatement: '{'\n";
            return false;
        }

        ////statements
        if(!parseStatements()){
            return false;
        }

        //'}'
        if(!match(202)){               
            cout<<"SE: ifStatement: '}'\n";
            return false;
        }
    }


    --indent;
    makeIndent(); result += "</ifStatement>\n";

    return true;

}



//whileStatement: 'while' '(' expression ')' '{' statements '}'
bool parseWhileStatement(){

    makeIndent(); result += "<whileStatement>\n";  
    ++indent;


    //'while'
    if(!match(120)){               
        cout<<"SE: whileStatement: 'while'\n";
        return false;
    }

    //'('
    if(!match(203)){               
        cout<<"SE: whileStatement: '('\n";
        return false;
    }

    //expression
    if(!parseExpression()){
        return false;
    }
    
    //')'
    if(!match(204)){               
        cout<<"SE: whileStatement: ')'\n";
        return false;
    }

    //'{'
    if(!match(201)){               
        cout<<"SE: whileStatement: '{'\n";
        return false;
    }

    //statements
    if(!parseStatements()){
        return false;
    }

    //'}'
    if(!match(202)){               
        cout<<"SE: whileStatement: '}'\n";
        return false;
    }


    --indent;
    makeIndent(); result += "</whileStatement>\n";

    return true;

}



//doStatement: 'do' subroutineCall ';'
bool parseDoStatement(){

    makeIndent(); result += "<doStatement>\n";  
    ++indent;


    //'do'
    if(!match(117)){               
        cout<<"SE: doStatement: 'do'\n";
        return false;
    }

    //subroutineName
    if(!match(500)){               
        cout<<"SE: doStatement: subroutineName\n";
        return false;
    }

    //subroutineCall
    if(!parseSubroutineCall()){
        return false;
    }
    
    //';'
    if(!match(209)){               
        cout<<"SE: doStatement: ';'\n";
        return false;
    }


    --indent;
    makeIndent(); result += "</doStatement>\n";

    return true;

}



//ReturnStatement: 'return' expression? ';'
bool parseReturnStatement(){

    makeIndent(); result += "<returnStatement>\n";  
    ++indent;


    //'return'
    if(!match(121)){               
        cout<<"SE: ReturnStatement: 'return'\n";
        return false;
    }

    //expression?
    if(tokenList[Index].code != 209){

        //expression
        if(!parseExpression()){
            return false;
        }
    }

    //';'
    if(!match(209)){               
        cout<<"SE: ReturnStatement: ';'\n";
        return false;
    }


    --indent;
    makeIndent(); result += "</returnStatement>\n";

    return true;

}