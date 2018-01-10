//Eric Berryhill


#include <cstdlib>
#include <iostream>
#include <fstream>
#include "lex.h"
#include "parse.h"
#include "eval.h"


using namespace std;

ifstream in;
lexeme *current;
lexeme *env;
int line = 1;

void displayLexeme(lexeme *l){
	if (l->getType().compare("NUMBER") == 0){
		cout << "int " << l->getInt();
	}
	else if (l->getType().compare("VAR") == 0){
		cout << "var " << l->getValue();
	}
	else if (l->getType().compare("STRING") == 0){
		cout << "string " << l->getValue();
	}
	else{
		cout << l->getType();
	}
}

int exploreTree(lexeme *t){
	cout << "\n   [*]";
	displayLexeme(t);
	cout << "( "<< t->getValue() << ")";
	cout << "\n  /   \\";
	cout << "\n /     \\\n";
	if(t->getLeft())
		displayLexeme(t->getLeft());
	cout << "\n         ";
	if(t->getRight())
		displayLexeme(t->getRight());
	cout << "\n";
	char ch = getchar();
	getchar();
	if(ch == 'a'){
		if(exploreTree(t->getLeft()))
			exploreTree(t);
	}
	else if(ch == 'd') {
		if(exploreTree(t->getRight()))
			exploreTree(t);
	}
	else if (ch == 's'){
		return 1;
	}
	else return 0;
}



int main(int argc, char **argv)
{
	in.open(argv[1]);
	current = lex();
	// cout << "done with first lexeme, current = " << current->getType() << endl;
	lexeme *tree = program();

	env = createEnv();
	// cout << "HERE" << endl;
	eval(tree, env);
	// cout << "\tEND OF PROGRAM ENVIRONMENT:" << endl;
	// displayEnv(env);
	// exploreTree(tree);
	return 0;
}