#ifndef _parse_h
#define _parse_h

#include "lex.h"
using namespace std;

void advance();
bool check(string type);
void errorMessage();
void matchNoAdvance(string type);

lexeme* match(string type);
lexeme* arrayDef();
lexeme* assignment();
lexeme* arglist();
lexeme* body();
lexeme* condexpr();
lexeme* condition();
lexeme* condop();
lexeme* definitions();
lexeme* elements();
lexeme* expressionone();
lexeme* expressiontwo();
lexeme* funcCall();
lexeme* funcDef();
lexeme* loop();
lexeme* opone();
lexeme* optwo();
lexeme* paramlist();
lexeme* program();
lexeme* statement();
lexeme* unary();
lexeme* varDef();

bool assignmentPending();
bool arglistPending();
bool arrayDefPending();
bool bodyPending();
bool condPending();
bool condopPending();
bool defPending();
bool elsePending();
bool statementPending();
bool expressiononePending();
bool expressiontwoPending();
bool funcCallPending();
bool funcDefPending();
bool loopPending();
bool oponePending();
bool optwoPending();
bool paramlistPending();
bool progPending();
bool statementPending();
bool unaryPending();
bool varDefPending();

#endif