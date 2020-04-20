#include "assemblerback.h"


unordered_map<string,int> symbol;
unordered_map<string,string> dest;
unordered_map<string,string> comp;
unordered_map<string,string> jump;


//Create Symbol Table
void createSymbolTab(string labelList[], int labelAdd[], int labelNum, string commandList[], int commandNum){
    
    //Predefined Symbols

    symbol["SP"]     = 0; 
    symbol["LCL"]    = 1; 
    symbol["ARG"]    = 2;
    symbol["THIS"]   = 3; 
    symbol["THAT"]   = 4; 
    symbol["R0"]     = 0;
    symbol["R1"]     = 1;
    symbol["R2"]     = 2;
    symbol["R3"]     = 3;
    symbol["R4"]     = 4;
    symbol["R5"]     = 5;
    symbol["R6"]     = 6;
    symbol["R7"]     = 7;
    symbol["R8"]     = 8;
    symbol["R9"]     = 9;
    symbol["R10"]    = 10;
    symbol["R11"]    = 11;
    symbol["R12"]    = 12;
    symbol["R13"]    = 13;
    symbol["R14"]    = 14;
    symbol["R15"]    = 15;
    symbol["SCREEN"] = 16384; 
    symbol["KBD"]    = 24576;

    //Label Symbols (xxx)

    for(int i=0; i<labelNum; ++i){
        symbol[labelList[i]] = labelAdd[i];
    }

    //Variable Symbols @xxx

    for(int i=0, mem=16; i<commandNum; ++i){

        if(commandList[i][0] == '@' && islower(commandList[i][1])){
            string var = commandList[i].substr(1);
            auto temp = symbol.find(var);

            if(temp == symbol.end()){
                symbol[var] = mem++;
            }
        }
    }

    /*
    //Print the Symbol Table
    {
        cout<<endl;
        for(auto i=symbol.cbegin(); i!=symbol.cend(); ++i){
            cout<<i->first<<" : "<<i->second<<endl;
        }
    }
    */
}


//Create Hash Table for [Mnemonic] -> [Code]
void createHashTab(){
    
    //DEST Mnemonics
    dest["M"]    = "001"; 
    dest["D"]    = "010"; 
    dest["MD"]   = "011"; 
    dest["A"]    = "100"; 
    dest["AM"]   = "101"; 
    dest["AD"]   = "110"; 
    dest["AMD"]  = "111"; 


    //COMP Mnemonics
    comp["0"]   = "0101010";
    comp["1"]   = "0111111";
    comp["-1"]  = "0111010";
    comp["D"]   = "0001100";
    comp["A"]   = "0110000"; 
    comp["M"]   = "1110000";
    comp["!D"]  = "0001101";
    comp["!A"]  = "0110001"; 
    comp["!M"]  = "1110001";
    comp["-D"]  = "0001111";
    comp["-A"]  = "0110011"; 
    comp["-M"]  = "1110011";
    comp["D+1"] = "0011111";
    comp["A+1"] = "0110111"; 
    comp["M+1"] = "1110111";
    comp["D-1"] = "0001110";
    comp["A-1"] = "0110010"; 
    comp["M-1"] = "1110010";
    comp["D+A"] = "0000010"; 
    comp["D+M"] = "1000010";
    comp["D-A"] = "0010011"; 
    comp["D-M"] = "1010011";
    comp["A-D"] = "0000111"; 
    comp["M-D"] = "1000111";
    comp["D&A"] = "0000000"; 
    comp["D&M"] = "1000000";
    comp["D|A"] = "0010101";
    comp["D|M"] = "1010101";    


    //JUMP Mnemonics
    jump["JGT"]  = "001";
    jump["JEQ"]  = "010";
    jump["JGE"]  = "011";
    jump["JLT"]  = "100";
    jump["JNE"]  = "101";
    jump["JLE"]  = "110";
    jump["JMP"]  = "111";

}


//A Instrucction: 0vvvvvvvvvvvvvvv
string transInsA(struct A_ins ins_A){

    auto temp = symbol.find(ins_A.label);

    //Integer constants
    if (temp == symbol.end()){
        return dec_bin(stoi(ins_A.label));
    } 

    //Symbols
    return dec_bin(temp->second);
}


//C Instruction: 111accccccdddjjj
string transInsC(struct C_ins ins_C){

    string temp = "111";

    temp += comp[ins_C.comp];
    
    if(ins_C.dest.length() > 0){
        temp += dest[ins_C.dest];
    }else{
        temp += "000";
    }

    if(ins_C.jump.length() > 0){
        temp += jump[ins_C.jump];
    }else{
        temp += "000";
    }   

    return temp;
}


//Convert decimal to binary
string dec_bin(int n){

    string temp = "";
    int x = -16;

    while(n){
        if(n&1){
            temp="1"+temp;
        }else{
            temp="0"+temp;
        }

        n=n>>1;
        ++x;
    }

    while(x < 0){
        temp="0"+temp;
        ++x;
    }

    return temp;
}