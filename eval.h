#ifndef _eval_h
#define _eval_h
#include "lex.h"


lexeme* eval(lexeme*, lexeme*);
lexeme* evalArgs(lexeme*, lexeme*);
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
lexeme* getClosureEnv(lexeme*);
lexeme* getClosureBody(lexeme*);
lexeme* getClosureParams(lexeme*);
lexeme* setArrayElements(lexeme*);

lexeme* createEnv();
lexeme* extendEnv(lexeme*, lexeme*, lexeme*);
lexeme* insert(lexeme*, lexeme*, lexeme*);
lexeme* lookupEnv(lexeme*, string);
lexeme* makeTable(lexeme*, lexeme*);
lexeme* updateEnv(lexeme*, lexeme*, lexeme*);
void displayEnv(lexeme*);
void displayLocalEnv(lexeme*);

#endif