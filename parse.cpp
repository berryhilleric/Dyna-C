/*These functions create parse trees that the evaluater then uses to evaluate the source code.*/

/*To do: still need to figure out how to deal with arrays. Need to figure
out the structure of the arrays*/
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "parse.h"

extern ifstream in;
extern lexeme *current;
extern int line;
lexeme *previous; //can use previous in my error message printing function


////Lexing Functions////

void advance()
{
	previous = current;
	current = lex();
}

bool check(string type)
{
	return current->getType().compare(type) == 0;
}

void errorMessage()
{
	if(current->getType().compare("ASSIGN") == 0)
	{
		cerr << "Line: "<< line << " -> syntax error, potentially missing assignment indicater \'@\' at the beginning of statement" << endl;
	}
	else if(current->getType().compare("OPAREN") == 0 && previous->getType().compare("ID") == 0)
	{
		cerr << "Line: "<< line << " -> syntax error, potentially missing function call indicater \'$\' at the beginning of statement" << endl;
	}
	else
	{
		cerr << "Line: " << line - 1 << " or " << line << ", syntax error -- the offending object: " << current->getType() << endl;
	}

	exit(0);
}

void matchNoAdvance(string type)
{
	if(!check(type))
	{
		errorMessage();
	}
}

lexeme* match(string type)
{
	// cout << type << endl;
	matchNoAdvance(type);
	lexeme* matchedLex = current;
	advance();
	return matchedLex;
}

////Parsing functions////

lexeme* arglist(){

	lexeme *args = new lexeme("ARGLIST");
	lexeme *u;
	if(expressiononePending())
	{
		u = expressionone();
		if(arglistPending())
		{
			args->cons(u, arglist());
			return args;
		}
		else {
			args->cons(u, NULL);
			return args;
		}

	}
	else
	{
		 u = match("VOID");
		 args->cons(u, NULL);
		 return args;
	}

}

lexeme* arrayDef()
{
	lexeme *ad = new lexeme("ARRAYDEF");
	lexeme *glue1 = new lexeme("GLUE");
	lexeme *glue2 = new lexeme("GLUE");
	match("ARRAY");
	ad->setLeft(match("ID"));
	match("OBRACE");
	glue1->cons(unary(), glue2); //unary() gets the index value
	match("CBRACE");
	match("ASSIGN");
	glue2->setLeft(elements());
	ad->setRight(glue1);
	return ad;
}

// lexeme* arrayIndex()
// {

// }

lexeme* assignment()
{
	match("ASSIGNMENT"); //array keyword
	lexeme* assign = new lexeme("ASSIGNMENT");
	lexeme* glue = new lexeme("GLUE");

	assign->setLeft(match("ID"));

	if(check("OBRACKET"))
	{
		match("OBRACKET");
		lexeme *u = unary();
		match("CBRACKET");
		lexeme *i = new lexeme("INDEX"); //lets the evaluator know that the variable is being indexed
		i->setLeft(u);
		i->setRight(glue);
		assign->setRight(i);	
	}
	else
	{
		assign->setRight(glue);
	}

	match("ASSIGN"); //actual '=' character
	glue->setLeft(expressionone());
	
	return assign;
}

lexeme* body()
{	
	lexeme *statedef;
	lexeme *b = new lexeme("BODY");
	if(statementPending())
	{
		statedef = statement();
	}
	else if(defPending())
	{
		statedef = definitions();
	}

	if(bodyPending())
	{
		b->cons(statedef, body());
	}
	else 
	{
		b->cons(statedef, NULL);
	}

	return b;
}

lexeme* condition() //CONVERTED
{
	lexeme *cond = new lexeme("IF");
	lexeme *glue1 = new lexeme("GLUE");
	match("IF");
	match("OPAREN");
	lexeme *c = condexpr();
	match("CPAREN");
	match("OBRACKET");
	lexeme *b1 = body();
	match("CBRACKET");
	if(check("ELSE"))
	{
		match("ELSE");
		if(condPending())
		{
			glue1->cons(b1, condition());
			cond->cons(c, glue1);
			return cond;
		}
		else
		{
			match("OBRACKET");
			lexeme *b2 = body();
			lexeme *glue2 = new lexeme("GLUE");
			match("CBRACKET");
			glue2->cons(b2, NULL);
			glue1->cons(b1, glue2);
			cond->cons(c, glue1);
		}
	}
	else
	{
		glue1->cons(b1, NULL);
		cond->cons(c, glue1);
	}
	return cond; 
}

lexeme* condexpr() //CONVERTED
{
	lexeme *condex = new lexeme("CONDEXPR");
	lexeme *u1 = expressionone();
	if(condopPending())
	{
		lexeme *glue1 = new lexeme("GLUE");
		lexeme *cop = condop();
		lexeme *glue2 = new lexeme("GLUE");
		lexeme *u2 = expressionone();
		glue2->cons(u2, NULL);
		glue1->cons(cop, glue2);
		condex->cons(u1, glue1);
	}
	else 
	{
		condex->cons(u1, NULL);
	}
	return condex;
}

lexeme* condop() //CONVERTED
{
	lexeme *c;
	if(check("GT"))
	{
		c = match("GT");
	}
	else if(check("GTE"))
	{
		c = match("GTE");
	}
	else if(check("LT"))
	{
		c = match("LT");
	}
	else if(check("LTE"))
	{
		c = match("LTE");
	}
	else if(check("EQUAL"))
	{
		c = match("EQUAL");
	}
	else if(check("NOTEQUAL"))
	{
		c = match("NOTEQUAL");
	}
	return c;
}

lexeme* definitions() //CONVERTED
{
	if (varDefPending())
	{
		lexeme* v = varDef();
		match("SEMI");
		return v;
	}
	else if(arrayDefPending())
	{
		lexeme* a = arrayDef();
		match("SEMI");
		return a;
	}
	else
	{
		return funcDef();
	}
}

lexeme* elements()
{
	lexeme *e = new lexeme("ELEMENTS");
	e->setLeft(match("NUMBER"));
	if(check("COMMA"))
	{
		match("COMMA");
		e->setRight(elements());
	}
	return e;
}

lexeme* expressionone()
{
	
	lexeme *e = expressiontwo();
	while(oponePending())
	{
		lexeme* o = opone();

		o->cons(e, expressiontwo());
		e = o;
	}
	
	return e;
}

lexeme* expressiontwo()
{
	lexeme* e = unary();
	while(optwoPending())
	{
		lexeme* o = optwo();
		o->cons(e, unary());
		e = o;
	}

	return e;
}


lexeme* funcCall() //CONVERTED
{
	lexeme *id, *args;
	lexeme *fc = new lexeme("FUNCCALL");
	lexeme *glue = new lexeme("GLUE");
	match("CALL");
	id = match("ID");
	match("OPAREN");
	args = arglist();
	match("CPAREN");
	glue->cons(args, NULL);
	fc->cons(id, glue);
	return fc;
}

lexeme* funcDef() //CONVERTED
{
	lexeme *id, *params, *b; 
	lexeme *fd = new lexeme("FUNCDEF");
	lexeme *glue1 = new lexeme("GLUE");
	lexeme *glue2 = new lexeme("GLUE");
	match("FUNCTION");
	id = match("ID");
	match("OPAREN");
	params = paramlist();
	match("CPAREN");
	match("OBRACKET");
	b = body();
	match("CBRACKET");
	glue2->cons(b, NULL);
	glue1->cons(params, glue2); 
	fd->cons(id, glue1);
	return fd;
}

lexeme* loop()	//CONVERTED
{
	lexeme *c, *b;
	lexeme *loop = new lexeme("WHILE");
	lexeme *glue = new lexeme("GLUE");
	match("WHILE");
	match("OPAREN");
	c = condexpr();
	match("CPAREN");
	match("OBRACKET");
	b = body();
	match("CBRACKET");
	glue->cons(b, NULL);
	loop->cons(c, glue);
	return loop;
}

lexeme* op(){
	if(oponePending())
	{
		return opone();
	}
	else if(optwoPending())
	{
		return optwo();
	}
	else
	{
		return match("ASSIGN");
	}
}

lexeme* opone()
{
	if(check("PLUS"))
		return match("PLUS");
	else
		return match("MINUS");
}

lexeme* optwo()
{
	if(check("MULTIPLY"))
		return match("MULTIPLY");
	else
		return match("DIVIDE");
}

lexeme* paramlist() //CONVERTED
{
	lexeme *pl = new lexeme("PARAMLIST");
	lexeme *u;
	if(check("ID"))
	{
		u = match("ID");

		if(paramlistPending())
		{
			pl->cons(u, paramlist());
		}
		else 
		{
			pl->cons(u, NULL);
		}
	}
	else
	{
		 u = match("VOID");
		 pl->cons(u, NULL);
	}

	return pl;
}

lexeme* program() //CONVERTED
{
	lexeme *statedef;
	lexeme *prog = new lexeme("PROGRAM");
	if(statementPending())
	{
		 statedef = statement();
	}
	else 
	{
		statedef = definitions();
	}

	if(progPending())
	{
		prog->cons(statedef, program());
		
	}
	else 
	{
		prog->cons(statedef, NULL);
	}

	return prog;
}



lexeme* statement() //CONVERTED
{
	if(condPending())
	{
		return condition();
	}
	else if(loopPending())
	{
		return loop();
	}
	else if(expressiononePending())
	{
		lexeme* e = expressionone();
		match("SEMI");

		return e;
	}
	else
	{
		lexeme* a = assignment();
		match("SEMI");
		return a;
	}
	
}

lexeme* unary() //CONVERTED
{

	if(check("ID"))
	{
		return match("ID");
	}
	else if(check("NUMBER"))
	{
		return match("NUMBER");
	}
	else if(check("STRING"))
	{
		return match("STRING");
	}
	else if(funcCallPending())
	{
		return funcCall();
	}
	else 
	{ 
		lexeme* o = match("OPAREN");
		o->setLeft(expressionone());
		match("CPAREN");
		return o;
	}
	
}

lexeme* varDef() 
{ 
	lexeme *vd = new lexeme("VARDEF");
	lexeme *glue = new lexeme("GLUE");
	match("VAR");
	lexeme *id = match("ID");
	match("ASSIGN");
	glue->cons(expressionone(), NULL);
	vd->cons(id, glue);
	return vd;
}


////PENDING FUNCS////



bool arglistPending()
{
	return unaryPending() || check("VOID");
}

bool arrayDefPending()
{
	return check("ARRAY");
}

bool assignmentPending()
{
	return check("ASSIGNMENT");
}

bool binaryPending()
{
	return unaryPending();
}

bool bodyPending()
{
	return statementPending() || defPending();
}

bool condPending()
{
	return check("IF");
}

bool condexprPending()
{
	return expressiononePending();
}

bool condopPending()
{
	return check("GT") || check("GTE") || check("LT") || check("LTE") || check("EQUAL") || check("NOTEQUAL");
}

bool defPending()
{
	return varDefPending() || funcDefPending() ;
}

bool expressiononePending()
{
	return  expressiontwoPending();
}

bool expressiontwoPending()
{
	return unaryPending();
}

bool funcCallPending()
{
	return check("CALL");
}

bool funcDefPending()
{
	return check("FUNCTION");
}

bool loopPending()
{
	return check("WHILE");
}

bool oponePending()
{
	return check("PLUS") || check("MINUS");
}

bool optwoPending()
{
	return check("MULTIPLY") || check("DIVIDE");
}

bool paramlistPending()
{
	return check("ID") || check("VOID");
}

bool progPending()
{
	return statementPending() || defPending();
}

bool statementPending()
{
	return condPending() || loopPending() || expressiononePending() || assignmentPending();
}

bool unaryPending()
{

	return check("ID") || check("NUMBER") || check("STRING") || funcCallPending() || check("OPAREN");
}

bool varDefPending()
{
	return check("VAR");
}
