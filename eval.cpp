//in the evalFuncCall function extendEnv takes the parameters and the argument
//values and uses makeTable to assign the values of the arguments to make a 
//new environment with the parameters have the values of the arguments


#include <cstdlib>
#include <iostream>
#include "eval.h"

extern lexeme* env;
extern lexeme* tree;
extern int line;

lexeme* eval(lexeme* tree, lexeme* env)
{

	if(tree->getType().compare("NUMBER") == 0 )
	{
		return tree;
	}
	else if(tree->getType().compare("STRING") == 0 )
	{
		return tree;
	}
	else if(tree->getType().compare("ID") == 0 )
	{
		return tree;
	}
	else if(tree->getType().compare("VOID") == 0)
	{
		return tree;
	}
	else if(tree->getType().compare("OPAREN") == 0 )
	{
		return eval(tree->getLeft(), env);
	}
	else if(tree->getType().compare("PLUS") == 0 )
	{
		return evalPlus(tree, env);
	}
	else if(tree->getType().compare("MINUS") == 0 )
	{
		return evalMinus(tree, env);
	}
	else if(tree->getType().compare("MULTIPLY") == 0 )
	{
		return evalMultiply(tree, env);
	}
	else if(tree->getType().compare("DIVIDE") == 0 )
	{
		return evalDivide(tree, env);
	}
	else if(tree->getType().compare("ASSIGNMENT") == 0 )
	{
		return evalAssignment(tree, env);
	}
	else if(tree->getType().compare("VARDEF") == 0 )
	{
		return evalVarDef(tree, env);
	}
	else if(tree->getType().compare("FUNCDEF") == 0 )
	{
		return evalFuncDef(tree, env);
	}
	else if(tree->getType().compare("IF") == 0 )
	{
		return evalIf(tree, env);
	}
	else if(tree->getType().compare("WHILE") == 0 )
	{
		return evalWhile(tree, env);
	}
	else if(tree->getType().compare("FUNCCALL") == 0 )
	{
		return evalFuncCall(tree, env);
	}
	else if(tree->getType().compare("PROGRAM") == 0  || tree->getType().compare("BODY") == 0 )
	{
		return evalBlock(tree, env);
	}
	else 
	{
		return NULL;
	}
}


lexeme* evalArgs(lexeme* tree, lexeme* env)
{
	lexeme* args = new lexeme("ARGS");
	lexeme* ptr = args; //a lexeme object that points to the from of the args and can be returned
	while(tree != NULL)
	{
		lexeme* a = eval(tree->getLeft(), env);
		
		args->setRight(new lexeme("GLUE"));

		if(a->getType().compare("ID") == 0)
		{
			a = lookupEnv(env, a->getValue());
		}
		else if(a->getType().compare("VOID") == 0)
		{
			a = new lexeme("VOID");
		}

		args->setLeft(a);
		args = args->getRight();
		
		tree = tree->getRight();
	}

	return ptr;
}

lexeme* evalarrayDef(lexeme* tree, lexeme* env)
{
	/*create a lexeme that will evaluate the list of elements and return a lexeme
	object that contains them. Then insert into the environment using the id of the 
	array.  Will need to create an evaluate elements function that creates that lexeme
	object containing the list of elements. I believe I only allow numbers in my array, so
	it would just return the list of numbers*/

	lexeme *e = setArrayElements(tree->getRight()->getRight()->getLeft()); //returns the elements
	insert(env, tree->getLeft(), e);
	return e;
}

lexeme* evalAssignment(lexeme* tree, lexeme* env)
{
	lexeme* value = eval(tree->getRight()->getLeft(), env);
	updateEnv(env, value, tree->getLeft());
	return value;
}

lexeme* evalBlock(lexeme* tree, lexeme* env)
{
	lexeme* r;
	while(tree != NULL)
	{
		// cout <<"EVAL BLOCK1" << endl;
		r = eval(tree->getLeft(), env);
		tree = tree->getRight();
		// cout <<"EVAL BLOCK2" << endl;
	}
	//displayEnv(env);
	//cout << "\tEND OF ENVIRONMENT" << endl;
	return r;
}

bool evalCondExpr(lexeme* tree, lexeme* env)
{
	lexeme* c1 = eval(tree->getLeft(), env);
	if(c1->getType().compare("ID") == 0)
	{
		c1 = lookupEnv(env, c1->getValue());
	}
	tree = tree->getRight();
	if(tree != NULL)
	{
		if(tree->getLeft()->getType().compare("LT") == 0)
		{

			lexeme *lhs = c1, *rhs;
			if(tree->getRight()->getLeft()->getType().compare("ID") == 0)
			{
				rhs = lookupEnv(env, tree->getRight()->getLeft()->getValue());
			}
			else
			{
				rhs = tree->getRight()->getLeft();
			}
			return lhs->getInt() < rhs->getInt();
		}
		else if(tree->getLeft()->getType().compare("GT") == 0)
		{
			
			lexeme *lhs = c1, *rhs;
			if(tree->getRight()->getLeft()->getType().compare("ID") == 0)
			{
				rhs = lookupEnv(env, tree->getRight()->getLeft()->getValue());
			}
			else
			{
				rhs = tree->getRight()->getLeft();
			}
			return lhs->getInt() > rhs->getInt();
		}
		else if(tree->getLeft()->getType().compare("LTE") == 0)
		{
			
			lexeme *lhs = c1, *rhs;
			if(tree->getRight()->getLeft()->getType().compare("ID") == 0)
			{
				rhs = lookupEnv(env, tree->getRight()->getLeft()->getValue());
			}
			else
			{
				rhs = tree->getRight()->getLeft();
			}
			return lhs->getInt() <= rhs->getInt();
		}
		else if(tree->getLeft()->getType().compare("GTE") == 0)
		{
			
			lexeme *lhs = c1, *rhs;
			if(tree->getRight()->getLeft()->getType().compare("ID") == 0)
			{
				rhs = lookupEnv(env, tree->getRight()->getLeft()->getValue());
			}
			else
			{
				rhs = tree->getRight()->getLeft();
			}
			return lhs->getInt() >= rhs->getInt();
		}
		else if(tree->getLeft()->getType().compare("EQUAL") == 0)
		{
			
			lexeme *lhs = c1, *rhs;
			if(tree->getRight()->getLeft()->getType().compare("ID") == 0)
			{
				rhs = lookupEnv(env, tree->getRight()->getLeft()->getValue());
			}
			else
			{
				rhs = tree->getRight()->getLeft();
			}
			return lhs->getInt() == rhs->getInt();
		}
		else 
		{
			
			lexeme *lhs = c1, *rhs;
			if(tree->getRight()->getLeft()->getType().compare("ID") == 0)
			{
				rhs = lookupEnv(env, tree->getRight()->getLeft()->getValue());
			}
			else
			{
				rhs = tree->getRight()->getLeft();
			}
			return lhs->getInt() != rhs->getInt();
		}
		
	}
	else
	{
		return c1->getInt();
	}
}

lexeme* evalDivide(lexeme* tree, lexeme* env)
{
	lexeme* op1 = eval(tree->getLeft(), env);

	if(op1->getType().compare("ID") == 0)
	{
		op1 = lookupEnv(env, op1->getValue());
	}

	lexeme* op2 = eval(tree->getRight(), env);

	if(op2->getType().compare("ID") == 0)
	{
		op2 = lookupEnv(env, op2->getValue());
	}

	if(op1->getType().compare("STRING") == 0 || op2->getType().compare("STRING") == 0)
	{
		cerr << "Line: " << line - 1 << " or " << line << " -> error, cannot used arithmetic operators on strings." << endl;
		exit(0);
	}

	if(op2->getInt() == 0)
	{
		cerr << "Line: " << line - 1 << " or " << line << " -> error, divide by zero." << endl;
		exit(0);
	}
	return new lexeme("NUMBER", op1->getInt() / op2->getInt());
}

lexeme* evalFuncCall(lexeme* tree, lexeme* env)
{
	//get args first before looking up funcId so you can service built-ins 
	//before searching for the function in the environment -- built-ins are
	//not save in the environment tables
	lexeme* a = evalArgs(tree->getRight()->getLeft(), env); 

	if(tree->getLeft()->getValue().compare("println") == 0)
	{
		return evalPrintln(a);
	}

	lexeme* c = lookupEnv(env, tree->getLeft()->getValue());
	
	lexeme* x = extendEnv(getClosureEnv(c), getClosureParams(c), a);
	return eval(getClosureBody(c), x);
	
}

lexeme* evalFuncDef(lexeme* tree, lexeme* env)
{
	lexeme* c = new lexeme("CLOSURE", tree, env);
	insert(env, tree->getLeft(), c);
	return c;
}

lexeme* evalIf(lexeme* tree, lexeme* env)
{
	bool b = evalCondExpr(tree->getLeft(), env);
	if(b)
	{
		env = extendEnv(env, NULL, NULL);
		evalBlock(tree->getRight()->getLeft(), env);
		env = env->getRight(); //get rid of the environment created by the previous if
	}

	

	if(!b && tree->getRight()->getRight() != NULL)
	{
		if(tree->getRight()->getRight()->getType().compare("IF") == 0)
		{
			evalIf(tree->getRight()->getRight(), env);
		}
		else
		{
			env = extendEnv(env, NULL, NULL);
			evalBlock(tree->getRight()->getRight()->getLeft(), env);
			env = env->getRight(); //get rid of the environment created by the previous if
		}
		
	}
	return NULL;
	
}

////////////////FUNC CALL HELPERS///////////////////////////////////////////////

lexeme* getClosureEnv(lexeme* c)
{
	return c->getRight();
}

lexeme* getClosureParams(lexeme* c)
{
	return c->getLeft()->getRight()->getLeft();
}

lexeme* getClosureBody(lexeme* c)
{
	return c->getLeft()->getRight()->getRight()->getLeft();
}

//////////////////////////////////////////////////////////////////////////////////

lexeme* evalMultiply(lexeme* tree, lexeme* env)
{
	lexeme* op1 = eval(tree->getLeft(), env);

	if(op1->getType().compare("ID") == 0)
	{
		op1 = lookupEnv(env, op1->getValue());
	}

	lexeme* op2 = eval(tree->getRight(), env);

	if(op2->getType().compare("ID") == 0)
	{
		op2 = lookupEnv(env, op2->getValue());
	}
	if(op1->getType().compare("STRING") == 0 || op2->getType().compare("STRING") == 0)
	{
		cerr << "Line: " << line - 1 << " or " << line << " -> error, cannot used arithmetic operators on strings." << endl;
		exit(0);
	}
	return new lexeme("NUMBER", op1->getInt() * op2->getInt());
}

lexeme* evalMinus(lexeme* tree, lexeme* env)
{
	lexeme* op1 = eval(tree->getLeft(), env);

	if(op1->getType().compare("ID") == 0)
	{
		op1 = lookupEnv(env, op1->getValue());
	}

	lexeme* op2 = eval(tree->getRight(), env);

	if(op2->getType().compare("ID") == 0)
	{
		op2 = lookupEnv(env, op2->getValue());
	}

	if(op1->getType().compare("STRING") == 0 || op2->getType().compare("STRING") == 0)
	{
		cerr << "Line: " << line - 1 << " or " << line << " -> error, cannot used arithmetic operators on strings." << endl;
		exit(0);
	}
	return new lexeme("NUMBER", op1->getInt() - op2->getInt());
}

lexeme* evalPlus(lexeme* tree, lexeme* env)
{
	lexeme* op1 = eval(tree->getLeft(), env);

	if(op1->getType().compare("ID") == 0)
	{
		op1 = lookupEnv(env, op1->getValue());
	}
	
	lexeme* op2 = eval(tree->getRight(), env);

	if(op2->getType().compare("ID") == 0)
	{
		op2 = lookupEnv(env, op2->getValue());
	}

	if(op1->getType().compare("STRING") == 0 || op2->getType().compare("STRING") == 0)
	{
		cerr << "Line: " << line - 1 << " or " << line << " -> error, cannot used arithmetic operators on strings." << endl;
		exit(0);
	}
	return new lexeme("NUMBER", op1->getInt() + op2->getInt());
}

lexeme* evalPrintln(lexeme* args)
{
	while(args->getLeft() != NULL)
	{

		if(args->getLeft()->getType().compare("ID") == 0)
		{
			args->setLeft(lookupEnv(env, args->getLeft()->getValue()));
		}

		if(args->getLeft()->getType().compare("NUMBER") == 0)
		{
			cout << args->getLeft()->getInt() << " ";
		}
		else if(args->getLeft()->getType().compare("STRING") == 0)
		{
			cout << args->getLeft()->getValue() << " ";
		}
		args = args->getRight();
	}
	cout << endl;
	return args;
}

lexeme* evalVarDef(lexeme* tree, lexeme* env)
{
	lexeme* value = eval(tree->getRight()->getLeft(), env);
	insert(env, tree->getLeft(), value);
	return value;
}

lexeme* evalWhile(lexeme* tree, lexeme* env)
{
	bool b = evalCondExpr(tree->getLeft(), env);
	 
	if(b)
	{
		env = extendEnv(env, NULL, NULL);
		while(b)
		{
			evalBlock(tree->getRight()->getLeft(), env);
			b = evalCondExpr(tree->getLeft(), env);
		}
			
		env = env->getRight(); //get rid of the environment created by the previous if
	}
	return NULL;
}

lexeme* setArrayElements(lexeme* e)
{
	lexeme* a = new lexeme("ARRAY");
	while(e != NULL)
	{
		a->getVector().push_back(e->getLeft()->getInt());
		e = e->getRight();
	}
	return a;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////


lexeme* makeTable(lexeme* variables, lexeme* values)
{
	return new lexeme("TABLE", variables, values);
}

lexeme* createEnv()
{
	return extendEnv(NULL, NULL, NULL);
}

lexeme* extendEnv(lexeme* env, lexeme* variables, lexeme* values)
{
	return new lexeme("ENV", makeTable(variables, values), env);
}

lexeme* lookupEnv(lexeme* env, string v)
{
	while(env != NULL)
	{
		lexeme* table = env->getLeft();
		lexeme* variables = table->getLeft();
		lexeme* values = table->getRight();
		
		while(variables != NULL)
		{
			
			if(variables->getLeft()->getValue().compare(v) == 0)
			{
				return values->getLeft();
			}
			//walk the list in parallel
			variables = variables->getRight();
			values = values->getRight();
			
		}
		
		env = env->getRight();
	}
	cout << "variable \"" << v << "\" is undefined" << endl;
	return NULL;
}

lexeme* updateEnv(lexeme* env, lexeme* newval, lexeme* v)
{
	while(env != NULL)
	{
		
		lexeme* table = env->getLeft();
		lexeme* variables = table->getLeft();
		lexeme* values = table->getRight();
		
		while(variables != NULL)
		{
			
			if(variables->getLeft()->getValue().compare(v->getValue()) == 0)
			{
				values->setLeft(newval);
				return NULL;
			}
			
			//walk the list in parallel
			variables = variables->getRight();
			values = values->getRight();
			
		}
		
		env = env->getRight();
	}

	cout << "variable \"" << v << "\" is undefined" << endl;
	return NULL;
}

lexeme* insert(lexeme* env, lexeme* variable, lexeme* value)
{
	lexeme* table = env->getLeft();
	table->setLeft(new lexeme("GLUE", variable, table->getLeft()));
	table->setRight(new lexeme("GLUE", value, table->getRight()));
	return value;

}

void displayLocalEnv(lexeme* env)
{

	lexeme* local = env->getLeft();
	lexeme* variables = local->getLeft();
	lexeme* values = local->getRight();
	while(variables != NULL)
	{
		if(variables->getLeft()->getType().compare("VOID") != 0)
			cout << variables->getLeft()->getValue() << ": ";
		
		if(values->getLeft()->getType().compare("NUMBER") == 0)
		{
			cout << values->getLeft()->getInt() << endl;
		}
		else if(values->getLeft()->getType().compare("CLOSURE") == 0)
		{
			cout << values->getLeft()->getType() << endl;
		}

		else
		{
			cout << values->getLeft()->getValue() << endl;
		}
		variables = variables->getRight();
		values = values->getRight();
	}
}

void displayEnv(lexeme* env)
{
	int i = 0;
	while(env != NULL)
	{
		cout << "E" << i <<":" << endl;
		displayLocalEnv(env);
		i++;
		env = env->getRight();
	}
}