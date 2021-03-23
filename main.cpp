#include <iostream>
#include <string>
#include <cmath>

#include "token.h"

using namespace std;

//
// Memory
//
// stores name and values of SimplePy variables
//
struct Memory {
    string   Name;
    string   Type;
    string   Value;
};

void executeStmt(Token program[], int& PC, Memory memory[], int& MC, int MemorySize);
void executeStmt1(Token program[], int& PC, Memory memory[], int& MC, int MemorySize);
void executeStmt2(Token program[], int& PC, Memory memory[], int& MC, int MemorySize);

//
// search
//
// Searches for the index of given SimplePy variable  
// Returns -1 if value not in memory 
//
int search(Memory* memory, int MemorySize, string variable){
    for (int i = 0; i < MemorySize; ++i){
        if (memory[i].Name == variable){
            return i; 
        }
    }
    return -1;
}

// 
// py_print
//
// performs Python print function
//
void py_print(Token program[], Memory memory[], int MemorySize, int indexer){
    if (program[indexer].ID != IDENTIFIER){
        if (program[indexer].ID == STR_LITERAL || program[indexer].ID == INT_LITERAL){
            cout << program[indexer].Value;
        }
                
        if (program[indexer+1].ID == COMMA){
            cout << " ";
            py_print(program, memory, MemorySize, indexer+2);
        }
        else {
            cout << endl;
        }
    }
    else {
        int x = search(memory, MemorySize, program[indexer].Value);
        cout << memory[x].Value;    
        
        if (program[indexer+1].ID == COMMA){
            cout << " ";
            py_print(program, memory, MemorySize, indexer+2);
        }
        else {
            cout << endl;
        }
    }    
} 

//
// input
//
// takes input from keyboard
//
string input(Token program[], Memory memory[], int MemorySize, int indexer){
    if (program[indexer].ID == INT_LITERAL || program[indexer].ID == STR_LITERAL){
        cout << program[indexer].Value << " ";
    }
    else if (search(memory, MemorySize, program[indexer].Value) != -1){
        cout << memory[search(memory, MemorySize, program[indexer].Value)].Value << " ";
    }
        
    memory[search(memory, MemorySize, program[indexer-4].Value)].Type = "str";
    
    string line;
    getline(cin, line);
    
    return line;
}

//
//intValid
//
// checks if string can be converted to type int 
//
int intValid(Token program[], Memory memory[], int MemorySize, int indexer, string s) {
    int x = search(memory, MemorySize, program[indexer-1].Value);
    
    for (int i = 0; i < (int) s.length(); i++)
        
        if (isdigit(s[i]) == false){
            if (x != -1){
                cout << "**Error: cannot convert s containing '" << memory[x].Value << "' @ (" << program[indexer-1].Line << ", " << program[indexer-1].Col << ") to 'int'" << endl;
            }
            else {
                cout << "**Error: cannot convert string '" << program[indexer-1].Value << "' @ (" << program[indexer-1].Line << ", " << program[indexer-1].Col << ") to 'int'" << endl;
            }
            
            return 0;    
        }
    return 1;
} 

//
// convert_int
//
// converts string variable to type int
// 
string convert_int(Token program[], Memory memory[], int MemorySize, int indexer, int is_var){
    int x = search(memory, MemorySize, program[indexer+2].Value);
    if (is_var == 0){
        return program[indexer].Value;
    }
    else {
        if (x != -1){
            return memory[search(memory, MemorySize, program[indexer+2].Value)].Value;
        }
        else {
            return program[indexer+2].Value;
        }
    }   
}

//
// convert_str
//
// converts int variable to type string
// 
string convert_str(Token program[], Memory memory[], int MemorySize, int indexer, int is_var){
    if (is_var == 0){
        return program[indexer].Value;
    }
    else {  
        return memory[search(memory, MemorySize, program[indexer+2].Value)].Value;
    } 
}

//
// type
//
// returns type of variable
//
string type(Token program[], Memory memory[], int MemorySize, int indexer){
    if (program[indexer].ID == STR_LITERAL){
        return "str";
    }
    else if (program[indexer].ID == INT_LITERAL){
        return "int";
    }
    else {
        return memory[search(memory, MemorySize, program[indexer].Value)].Type;
    }
}


//
// semantic_checking
//
// runs input(), int(), string() and type() functions
//
void semantic_checking(Token program[], Memory memory[], int MemorySize, int indexer){
    if (program[indexer].Value == "input"){
        memory[search(memory, MemorySize, program[indexer-2].Value)].Type = "int";
        memory[search(memory, MemorySize, program[indexer-2].Value)].Value = input(program, memory, MemorySize, indexer+2);
    }
    else if (program[indexer].Value == "int"){
        int x;
        string y;
        
        if(program[indexer+2].ID == STR_LITERAL){
            x = intValid(program, memory, MemorySize, indexer+3, program[indexer+2].Value);
            y = convert_int(program, memory, MemorySize, indexer+2, 0);
        }
        else if(program[indexer+2].ID == INT_LITERAL){
            x = 1;
            y = program[indexer+2].Value;
        }
        else {
            x = intValid(program, memory, MemorySize, indexer+3, memory[search(memory, MemorySize, program[indexer+2].Value)].Value);
            y = convert_int(program, memory, MemorySize, indexer, 1);
            
        }
        if (x == 1){
            memory[search(memory, MemorySize, program[indexer-2].Value)].Type = "int";
            memory[search(memory, MemorySize, program[indexer-2].Value)].Value = y;
        }
        else {
            memory[search(memory, MemorySize, program[indexer-2].Value)].Value = "0";
        }
    }
    else if (program[indexer].Value == "str"){
        string y;
        if(program[indexer+2].ID == INT_LITERAL){
            y = convert_str(program, memory, MemorySize, indexer+2, 0);
        }
        else {
            y = convert_int(program, memory, MemorySize, indexer, 1);
        }
        
        memory[search(memory, MemorySize, program[indexer-2].Value)].Type = "str";
        memory[search(memory, MemorySize, program[indexer-2].Value)].Value = y;
    }
    else if (program[indexer].Value == "type"){
        string y = type(program, memory, MemorySize, indexer+2);
        memory[search(memory, MemorySize, program[indexer-2].Value)].Type = "str";
        memory[search(memory, MemorySize, program[indexer-2].Value)].Value = y;
    }
}


//
// operational_identifier
//
// performs operations on int type variables and makes comparisions
// supports +, -, *, **, /, %, <, <=, >, >=, ==, !=
//
void operational_identifier(Token program[], Memory memory[], int MemorySize, int indexer){
    int val1, val2;
    if (program[indexer-1].ID == IDENTIFIER){
        val1 = stoi(memory[search(memory, MemorySize, program[indexer-1].Value)].Value);
    }
    else {
        val1 = stoi(program[indexer-1].Value);
    }
    if (program[indexer+1].ID == IDENTIFIER){
        val2 = stoi(memory[search(memory, MemorySize, program[indexer+1].Value)].Value);
    }
    else {
        val2 = stoi(program[indexer+1].Value);
    } 
    if (program[indexer].ID == PLUS){
        memory[search(memory, MemorySize, program[indexer-3].Value)].Value = to_string(val1 + val2);
    }
    else if (program[indexer].ID == MINUS){
        memory[search(memory, MemorySize, program[indexer-3].Value)].Value = to_string(val1 - val2);
    }
    else if (program[indexer].ID == MULT){
        memory[search(memory, MemorySize, program[indexer-3].Value)].Value = to_string(val1 * val2);
    }
    else if (program[indexer].ID == POWER){
        int power = pow(double(val1), val2);
        memory[search(memory, MemorySize, program[indexer-3].Value)].Value = to_string(power);
    }
    else if (program[indexer].ID == DIV){
        if (val2 != 0){
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = to_string(val1 / val2);
        }
        else {
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "0";
        }
    }
    else if (program[indexer].ID == MOD){
        if (val2 != 0){
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = to_string(val1 % val2);
        }
        else {
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "0";
        }

    }
    else if (program[indexer].ID == LT){
        if (val1 < val2){
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "1";
        }
        else {
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "0";
        }
    }
    else if (program[indexer].ID == LTE){
        if (val1 <= val2){
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "1";
        }
        else {
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "0";
        }
    }
    else if (program[indexer].ID == GT){
        if (val1 > val2){
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "1";
        }
        else {
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "0";
        }
    }
    else if (program[indexer].ID == GTE){
        if (val1 >= val2){
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "1";
        }
        else {
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "0";
        }
    }
    else if (program[indexer].ID == EQUAL_EQUAL){
        if (val1 == val2){
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "1";
        }
        else {
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "0";
        }
    }
    else if (program[indexer].ID == NOT_EQUAL){
        if (val1 != val2){
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "1";
        }
        else {
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "0";
        }
    }
}

//
// str_literal
//
// performs string concantenation and compares string values
// supports +, <, <=, >, >=, ==, !=
//
void str_literal(Token program[], Memory memory[], int MemorySize, int indexer){
    string val1, val2;
    if (program[indexer-1].ID == IDENTIFIER){
        val1 = memory[search(memory, MemorySize, program[indexer-1].Value)].Value;
    }
    else {
        val1 = program[indexer-1].Value;
    }
    if (program[indexer+1].ID == IDENTIFIER){
        val2 = memory[search(memory, MemorySize, program[indexer+1].Value)].Value;
    }
    else {
        val2 = program[indexer+1].Value;
    } 
    
    memory[search(memory, MemorySize, program[indexer-3].Value)].Type = "str";
    
    if (program[indexer].ID == PLUS){
        memory[search(memory, MemorySize, program[indexer-3].Value)].Value = val1 + val2;
    }
    else if (program[indexer].ID == LT){
        if (val1 < val2){
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "1";
        }
        else {
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "0";
        }
    }
    else if (program[indexer].ID == LTE){
        if (val1 <= val2){
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "1";
        }
        else {
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "0";
        }
    }
    else if (program[indexer].ID == GT){
        if (val1 > val2){
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "1";
        }
        else {
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "0";
        }
    }
    else if (program[indexer].ID == GTE){
        if (val1 >= val2){
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "1";
        }
        else {
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "0";
        }
    }
    else if (program[indexer].ID == EQUAL_EQUAL){
        if (val1 == val2){
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "1";
        }
        else {
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "0";
        }
    }
    else if (program[indexer].ID == NOT_EQUAL){
        if (val1 != val2){
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "1";
        }
        else {
            memory[search(memory, MemorySize, program[indexer-3].Value)].Value = "0";
        }
    }   
}

//
// executeStmt1
//
//
void executeStmt1(Token program[], int& PC, Memory memory[], int& MC, int MemorySize){
    int x = search(memory, MemorySize, program[PC].Value);
        if (x == -1){
            memory[MC].Name = program[PC].Value;
            if (program[PC+2].ID == STR_LITERAL && program[PC+3].ID == IDENTIFIER){
                memory[MC].Type = "str";
                memory[MC].Value = program[PC+2].Value;
            }
            else if (program[PC+2].ID == INT_LITERAL && program[PC+3].ID == IF_KEYW){
                memory[MC].Type = "int";
                memory[MC].Value = program[PC+2].Value;
            }
            else if (program[PC+3].ID != IDENTIFIER){
                if (program[PC+2].ID == INT_LITERAL && program[PC+4].ID == STR_LITERAL){
                    cout << "**Error: type error with '" << program[PC+3].Value << "' @ (" << program[PC+3].Line << ", " << program[PC+3].Col << "), expr is 'int' + 'str'" << endl;
                    memory[search(memory, MemorySize, program[PC].Value)].Type = "int";
                    memory[search(memory, MemorySize, program[PC].Value)].Value = "0";
                }
                else if (program[PC+2].ID == STR_LITERAL && program[PC+4].ID == INT_LITERAL){
                    cout << "**Error: type error with '" << program[PC+3].Value << "' @ (" << program[PC+3].Line << ", " << program[PC+3].Col << "), expr is 'str' + 'int'" << endl;
                    memory[search(memory, MemorySize, program[PC].Value)].Type = "int";
                    memory[search(memory, MemorySize, program[PC].Value)].Value = "0";
                }
                else if (program[PC+2].Value == "input" || program[PC+2].Value == "int" || program[PC+2].Value == "str" || program[PC+2].Value == "type"){
                    semantic_checking(program, memory, MemorySize, PC);
                }
                else if (program[PC+2].ID == INT_LITERAL || program[PC+4].ID == INT_LITERAL){
                    memory[MC].Type = "int";
                    operational_identifier(program, memory, MemorySize, PC+3);
                }
                else if (program[PC+2].ID == STR_LITERAL || program[PC+4].ID == STR_LITERAL) {
                    if (program[PC+2].ID == IDENTIFIER){
                        if (memory[search(memory, MemorySize, program[PC+2].Value)].Type == "int" && program[PC+4].ID == STR_LITERAL){
                            cout << "**Error: type error with '" << program[PC+3].Value << "' @ (" << program[PC+3].Line << ", " << program[PC+3].Col << "), expr is 'int' + 'str' " << endl;
                            memory[MC].Type = "int";
                            memory[MC].Value = "0";
                        }
                        else {
                            str_literal(program, memory, MemorySize, PC+3);
                        }
                    }
                    else if (program[PC+4].ID == IDENTIFIER){
                        if (memory[search(memory, MemorySize, program[PC+4].Value)].Type == "int" && program[PC+2].ID == STR_LITERAL){
                            cout << "**Error: type error with '" << program[PC+3].Value << "' @ (" << program[PC+3].Line << ", " << program[PC+3].Col << "), expr is 'str' + 'int' " << endl;
                            memory[MC].Type = "int";
                            memory[MC].Value = "0";
                        }
                        else {
                            str_literal(program, memory, MemorySize, PC+3);
                        }
                    }
                    else if (program[PC+3].ID == PLUS || program[PC+3].ID == LT || program[PC+3].ID == LTE || program[PC+3].ID == GT || program[PC+3].ID == GTE || program[PC+3].ID == EQUAL_EQUAL || program[PC+3].ID == NOT_EQUAL){
                        str_literal(program, memory, MemorySize, PC+3);
                    }
                    else if (program[PC+2].Value == "input" || program[PC+2].Value == "int" || program[PC+2].Value == "str" || program[PC+2].Value == "type"){
                        semantic_checking(program, memory, MemorySize, PC);
                    }    
                    else if (program[PC+3].ID == INDENT || program[PC+3].ID == UNDENT || program[PC+3].ID == IF_KEYW){
                        memory[MC].Value = program[PC+2].Value;
                    }
                    else {
                        cout << "**Error: type error with '" << program[PC+3].Value << "' @ (" << program[PC+3].Line << ", " << program[PC+3].Col << "), expr is 'str' + 'str'" << endl;
                        memory[MC].Value = "0";
                    }
                }
                else if ((memory[search(memory, MemorySize, program[PC+2].Value)].Type == "int" && memory[search(memory, MemorySize, program[PC+4].Value)].Type == "str") || (memory[search(memory, MemorySize, program[PC+2].Value)].Type == "str" && memory[search(memory, MemorySize, program[PC+4].Value)].Type == "int")){
                    cout << "**Error: type error with '" << program[PC+3].Value << "' @ (" << program[PC+3].Line << ", " << program[PC+3].Col << "), expr is '" << memory[search(memory, MemorySize, program[PC+2].Value)].Type << "' + '" << memory[search(memory, MemorySize, program[PC+4].Value)].Type << "'" << endl;
                    memory[MC].Type = "int";
                    memory[MC].Value = "0";
                }
                else if (memory[search(memory, MemorySize, program[PC+2].Value)].Type == "int" || memory[search(memory, MemorySize, program[PC+4].Value)].Type == "int"){
                    if ((memory[search(memory, MemorySize, program[PC+2].Value)].Type == "int" && memory[search(memory, MemorySize, program[PC+4].Value)].Type == "str") || (memory[search(memory, MemorySize, program[PC+2].Value)].Type == "str" && memory[search(memory, MemorySize, program[PC+4].Value)].Type == "int")){
                        cout << "**Error: type error with '" << program[PC+3].Value << "' @ (" << program[PC+3].Line << ", " << program[PC+3].Col << "), expr is '" << memory[search(memory, MemorySize, program[PC+2].Value)].Type << "' + '" << memory[search(memory, MemorySize, program[PC+4].Value)].Type << "'" << endl;
                        memory[MC].Type = "int";
                        memory[MC].Value = "0";
                    }
                    else {
                        memory[MC].Type = "int";
                        operational_identifier(program, memory, MemorySize, PC+3);
                    }
                } 
                else if (memory[search(memory, MemorySize, program[PC+2].Value)].Type == "str" || memory[search(memory, MemorySize, program[PC+4].Value)].Type == "str"){
                    if (program[PC+3].ID == PLUS || program[PC+3].ID == LT || program[PC+3].ID == LTE || program[PC+3].ID == GT || program[PC+3].ID == GTE || program[PC+3].ID == EQUAL_EQUAL || program[PC+3].ID == NOT_EQUAL){
                        str_literal(program, memory, MemorySize, PC+3);
                    }
                    else {
                        cout << "**Error: type error with '" << program[PC+3].Value << "' @ (" << program[PC+3].Line << ", " << program[PC+3].Col << "), expr is 'str' + 'str'" << endl;
                    }
                }
            }
            else if (program[PC+2].ID == INT_LITERAL){
                memory[MC].Type = "int";
                memory[MC].Value = program[PC+2].Value;
            }
            else if (memory[search(memory, MemorySize, program[PC+2].Value)].Type == "int"){
                memory[MC].Type = "int";
                memory[MC].Value = memory[search(memory, MemorySize, program[PC+2].Value)].Value;
            }
            else if (memory[search(memory, MemorySize, program[PC+2].Value)].Type == "str"){
                memory[MC].Type = "str";
                memory[MC].Value = memory[search(memory, MemorySize, program[PC+2].Value)].Value;
            }
        }    
        else {
            executeStmt2(program, PC, memory, MC, MemorySize);
        }
    
}    

//
// executeStmt2
//
//
void executeStmt2(Token program[], int& PC, Memory memory[], int& MC, int MemorySize){
    if (program[PC+2].ID == STR_LITERAL && program[PC+3].ID == IDENTIFIER){
                memory[search(memory, MemorySize, program[PC].Value)].Type = "str";
                memory[search(memory, MemorySize, program[PC].Value)].Value = program[PC+2].Value;
            }
            else if (program[PC+2].ID == INT_LITERAL && program[PC+3].ID == IDENTIFIER){
                memory[search(memory, MemorySize, program[PC].Value)].Type = "int";
                memory[search(memory, MemorySize, program[PC].Value)].Value = program[PC+2].Value;
            }
            else if (program[PC+2].Value == "input" || program[PC+2].Value == "int" || program[PC+2].Value == "str" || program[PC+2].Value == "type"){
                semantic_checking(program, memory, MemorySize, PC);
            }
            else if (program[PC+2].ID == INT_LITERAL && program[PC+4].ID == STR_LITERAL){
                cout << "**Error: type error with '" << program[PC+3].Value << "' @ (" << program[PC+3].Line << ", " << program[PC+3].Col << "), expr is 'int' + 'str'" << endl;
                memory[search(memory, MemorySize, program[PC].Value)].Type = "int";
                memory[search(memory, MemorySize, program[PC].Value)].Value = "0";
            }
            else if (program[PC+2].ID == STR_LITERAL && program[PC+4].ID == INT_LITERAL){
                cout << "**Error: type error with '" << program[PC+3].Value << "' @ (" << program[PC+3].Line << ", " << program[PC+3].Col << "), expr is 'str' + 'int'" << endl;
                memory[search(memory, MemorySize, program[PC].Value)].Type = "int";
                memory[search(memory, MemorySize, program[PC].Value)].Value = "0";
            }
            else if (program[PC+2].ID == INT_LITERAL || program[PC+4].ID == INT_LITERAL){
                if (program[PC+2].ID == IDENTIFIER){
                    if (memory[search(memory, MemorySize, program[PC+2].Value)].Type == "str" && program[PC+4].ID == INT_LITERAL){
                        cout << "**Error: type error with '" << program[PC+3].Value << "' @ (" << program[PC+3].Line << ", " << program[PC+3].Col << "), expr is 'str' + 'int' " << endl;
                        memory[search(memory, MemorySize, program[PC].Value)].Type = "int";
                        memory[search(memory, MemorySize, program[PC].Value)].Value = "0";
                    }
                    else {
                        operational_identifier(program, memory, MemorySize, PC+3);
                    }
                }
                else if (program[PC+4].ID == IDENTIFIER){
                    if (memory[search(memory, MemorySize, program[PC+4].Value)].Type == "str" && program[PC+2].ID == INT_LITERAL){
                        cout << "**Error: type error with '" << program[PC+3].Value << "' @ (" << program[PC+3].Line << ", " << program[PC+3].Col << "), expr is 'int' + 'str' " << endl;
                        memory[search(memory, MemorySize, program[PC].Value)].Type = "int";
                        memory[search(memory, MemorySize, program[PC].Value)].Value = "0";
                    }
                    else {
                        operational_identifier(program, memory, MemorySize, PC+3);
                    }
                }
                else {
                    memory[search(memory, MemorySize, program[PC].Value)].Value = "int";
                    operational_identifier(program, memory, MemorySize, PC+3);
                }
            }
            else if (program[PC+2].ID == STR_LITERAL || program[PC+4].ID == STR_LITERAL) { 
                if (program[PC+2].ID == IDENTIFIER){
                    if (memory[search(memory, MemorySize, program[PC+2].Value)].Type == "int" && program[PC+4].ID == STR_LITERAL){
                        cout << "**Error: type error with '" << program[PC+3].Value << "' @ (" << program[PC+3].Line << ", " << program[PC+3].Col << "), expr is 'int' + 'str' " << endl;
                        memory[search(memory, MemorySize, program[PC].Value)].Type = "int";
                        memory[search(memory, MemorySize, program[PC].Value)].Value = "0";
                    }
                    else {
                        str_literal(program, memory, MemorySize, PC+3);
                    }
                }
                else if (program[PC+4].ID == IDENTIFIER){
                    if (memory[search(memory, MemorySize, program[PC+4].Value)].Type == "int" && program[PC+2].ID == STR_LITERAL){
                        cout << "**Error: type error with '" << program[PC+3].Value << "' @ (" << program[PC+3].Line << ", " << program[PC+3].Col << "), expr is 'str' + 'int' " << endl;
                        memory[search(memory, MemorySize, program[PC].Value)].Type = "int";
                        memory[search(memory, MemorySize, program[PC].Value)].Value = "0";
                    }
                    else {
                        str_literal(program, memory, MemorySize, PC+3);
                    }
                }
                else {
                    str_literal(program, memory, MemorySize, PC+3);
                }
            }
            else if (memory[search(memory, MemorySize, program[PC+2].Value)].Type == "int"){
                if (program[PC+3].ID != IDENTIFIER){
                    operational_identifier(program, memory, MemorySize, PC+3);
                }
                else {
                    memory[search(memory, MemorySize, program[PC].Value)].Type = "int";
                    memory[search(memory, MemorySize, program[PC].Value)].Value = memory[search(memory, MemorySize, program[PC+2].Value)].Value;
                }
            }
            else if ((memory[search(memory, MemorySize, program[PC+2].Value)].Type == "int" && memory[search(memory, MemorySize, program[PC+4].Value)].Type == "str") || (memory[search(memory, MemorySize, program[PC+2].Value)].Type == "str" && memory[search(memory, MemorySize, program[PC+4].Value)].Type == "int") || (memory[search(memory, MemorySize, program[PC+2].Value)].Type == "int" && program[PC+4].ID == STR_LITERAL) || (program[PC+2].ID == STR_LITERAL && memory[search(memory, MemorySize, program[PC+4].Value)].Type == "int")){
                cout << "**Error: type error with '" << program[PC+3].Value << "' @ (" << program[PC+3].Line << ", " << program[PC+3].Col << "), expr is '" << memory[search(memory, MemorySize, program[PC+2].Value)].Type << "' + '" << memory[search(memory, MemorySize, program[PC+4].Value)].Type << "'" << endl;
                memory[search(memory, MemorySize, program[PC].Value)].Type = "int";
                memory[search(memory, MemorySize, program[PC].Value)].Value = "0";
            }
            else if (memory[search(memory, MemorySize, program[PC+2].Value)].Type == "str"){
                str_literal(program, memory, MemorySize, PC+3);
            }
}  

//
// executeStmt
//
// executes SimplePy program
//
void executeStmt(Token program[], int& PC, Memory memory[], int& MC, int MemorySize){
    if (program[PC].ID == IDENTIFIER && program[PC+1].ID == EQUAL){ 
        executeStmt1(program, PC, memory, MC, MemorySize);
    }    
    else if (program[PC].Value == "print"){
        py_print(program, memory, MemorySize, PC+2);
    }
    else if (program[PC].Value == "input" || program[PC].Value == "int" || program[PC].Value == "str" || program[PC].Value == "type"){
        semantic_checking(program, memory, MemorySize, PC);
    }
}

// ##################################################################################
//
// main filename memorysize
//
int main(int argc, char* argv[])
{
   // 
   // 1. process command-line args:
   //
   // argv[0]: ./main
   // argv[1]: filename.py
   // argv[2]: memorysize
   //
    if (argc != 3) {
        cout << "usage: ./main filename.py memorysize" << endl;
        return 0;
    }

    string filename(argv[1]);  // this is your SimplePy program file
    int    MemorySize = stoi(argv[2]);  // this is size for Memory array

    
    int    ProgramSize;
    Token* program = inputSimplePy(filename, ProgramSize);

    if (program == nullptr){
        cout << "**Error, unable to open program file '" << filename << "'" << endl;
        return 0;
    }

    Memory* memory = new Memory[MemorySize];
    
    int PC = 0; // “program counter”
    int MC = 0; // "memory counter"
    
    while (program[PC].ID != TokenID::EOS){
        executeStmt(program, PC, memory, MC, MemorySize);
        
        if (memory[MC].Name != ""){           
            MC++;
        }
        
        PC++;
    }
    
    delete[] memory;
    delete[] program;
    
    return 0;
}
