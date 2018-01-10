#ifndef _eval_h
#define _eval_h
#include "lex.h"


lexeme* eval(lexeme*, lexeme*);
lexeme* evalArgs(lexeme*, lexeme*);
lexeme* evalarrayDef(lexeme*, lexeme*);
lexeme* evalAssignment(lexeme*, lexeme*);
lexeme* evalBlock(lexeme*, lexeme*);
bool evalCondExpr(lexeme*, lexeme*);
lexeme* evalDivide(lexeme*, lexeme*);
lexeme* evalFuncCall(lexeme*, lexeme*);
lexeme* evalFuncDef(lexeme*, lexeme*);
lexeme* evalIf(lexeme*, lexeme*);
lexeme* evalMultiply(lexeme*, lexeme*);
lexeme* evalMinus(lexeme*, lexeme*);
lexeme* evalPlus(lexeme*, lexeme*);
lexeme* evalPrintln(lexeme*);
lexeme* evalSimpleOp(lexeme*, lexeme*);
lexeme* evalWhile(lexeme*, lexeme*);
lexeme* evalVarDef(lexeme*, lexeme*);
//pushes the values into the ARRAY lexemes vector variables to create a list
lexeme* setArrayElements(lexeme*);


//every closure points to the environment in which is defined -- this allows you to determine scope and shows 
//which variables are available to the function either locally or globally
lexeme* getClosureEnv(lexeme*);
//every closure points to its function definition in the parse tree -- allows you to jump to the exact spot
//inthe parse tree to evaluate that functions body for the function call
lexeme* getClosureBody(lexeme*);
lexeme* getClosureParams(lexeme*);


//creates a brand new environment -- null
lexeme* createEnv();
//creates a brand new environment -- used for function calls, if statements, while loops
lexeme* extendEnv(lexeme*, lexeme*, lexeme*);
//adds a variable to the current environments table of variables/values
lexeme* insert(lexeme*, lexeme*, lexeme*);
//retrieves a specified variables value
lexeme* lookupEnv(lexeme*, lexeme*);
//creates a new table of variables and their values for the current environment
lexeme* makeTable(lexeme*, lexeme*);
//called when a variables value needs updating
lexeme* updateEnv(lexeme*, lexeme*, lexeme*);
//displays every environments variables
void displayEnv(lexeme*);
//displays the current environements variables
void displayLocalEnv(lexeme*);

#endif