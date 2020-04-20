#include "SyntaxAnalysis.h"



unordered_map <string,entry> ST_class;
unordered_map <string,entry> ST_subRoutine;

int staticIndex, fieldIndex, localIndex, argIndex; 
extern int label_Loop, label_Condition;
extern int num_FormalParams, num_Local;
    


void ST_addEntry(string name, string type, int scope){
    
    struct entry ST_entry;
    ST_entry.type  = type;
    
    switch(scope){

        case STATIC: 
            ST_entry.scope = "static";
            ST_entry.index = to_string(staticIndex++);
            ST_class[name] = ST_entry;
        break;

        case FIELD: 
            ST_entry.scope = "this";
            ST_entry.index = to_string(fieldIndex++);
            ST_class[name] = ST_entry;
        break;
        
        case ARG:
            ST_entry.scope = "argument";
            ST_entry.index = to_string(argIndex++);
            ST_subRoutine[name] = ST_entry;
        break;
        
        case LOCAL: 
            ST_entry.scope = "local";
            ST_entry.index = to_string(localIndex++);
            ST_subRoutine[name] = ST_entry;
        break;
        
    }

    //cout<<endl<<name<<" - "<<ST_entry.type<<" - "<<ST_entry.scope<<" - "<<ST_entry.index<<endl;
}



struct entry* ST_search(string var){

    auto ST_searchVar = ST_subRoutine.find(var);                
    if(ST_searchVar == ST_subRoutine.end()){

        ST_searchVar  = ST_class.find(var);
        if(ST_searchVar == ST_class.end()){
            return NULL;
        }
    }

    return &(ST_searchVar->second);
}



void ST_clear(){

    argIndex = localIndex = 0;    
    num_FormalParams = num_Local = 0;
    label_Condition = label_Loop = 0;
    
    ST_subRoutine.clear();
}