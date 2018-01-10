/*Issues to address: when the lexer gets to the very last lexeme in the program, it reads it correctly
but when we call skipWhiteSpace, we encounter the eof charcter which causes the following eof conditional
to pass and return the ENDOFINPUT lexeme before the last lexeme is made. One possible solution is to remove 
the if statment and create an ENDOFINPUT lexeme after the while loop terminiates. Not sure yet how I'm going 
to go about this yet.

Still need to account for comments to

Rack brain to figure out what else is needed*/




#include <cstdlib>
#include <cctype>
#include <iostream>
#include "lex.h"

extern lexeme* current;
extern ifstream in;
extern int line;


lexeme* lex()
{
	

	char ch;
	in.get(ch);	

	// cout << "current char: " << ch << endl;
	

	if(ch == '#') //deal with comments
    {
    	in.get(ch);
    	while(ch != '#')
    	{
    		in.get(ch);
    		if(in.eof())
	    	{
	    		cerr << "Line: " << line << " -> syntax error, unterminated comment" << endl;
	    		exit(0);
	    	}
    	}
    	return lex();
    }

	if(isspace(ch)) // this check prevents automatically reading the next character if its not needed
		skipWhiteSpace();

	if(isspace(ch)) 
		in.get(ch);

	switch(ch) 
    { 
	    // single character tokens 
	    case '(': return new lexeme("OPAREN"); 
	    case ')': return new lexeme("CPAREN"); 
	    case '[': return new lexeme("OBRACE"); 
	    case ']': return new lexeme("CBRACE");
	    case '{': return new lexeme("OBRACKET"); 
	    case '}': return new lexeme("CBRACKET"); 
	    case '+': return new lexeme("PLUS"); 
	    case '-': return new lexeme("MINUS");
	    case '*': return new lexeme("MULTIPLY"); 
	    case '/': return new lexeme("DIVIDE"); 
	    case ';': return new lexeme("SEMI");
	    case ',': return new lexeme("COMMA");
	    case '$': return new lexeme("CALL");
	    case '@': return new lexeme("ASSIGNMENT");
	    // case '~': return new lexeme("ARRAYINDEX");
	    case '!': 
	    	in.get(ch);
	    	if(ch == '=')
	    		return new lexeme("NOTEQUAL");
	    	else
	    	{
	    		in.unget();
	    		return new lexeme("NOT");
	    	}
	    	
	    case '<': 
	    	in.get(ch);
	    	if(ch == '=')
	    		return new lexeme("LTE");
	    	else 
	    	{
	    		in.unget();
	    		return new lexeme("LT"); 
	    	}
	    case '>': 
	    	in.get(ch);
	    	if(ch == '=')
	    		return new lexeme("GTE");
	    	else
	    	{
	    		in.unget();
	    		return new lexeme("GT"); 
	    	}
	    	
	    case '=': 
	    	in.get(ch);
	    	if(ch == '=')
	    		return new lexeme("EQUAL");
	    	else
	    	{
	    		in.unget();
	    		return new lexeme("ASSIGN"); 
	    	} 

	    default:

	    	if(isdigit(ch))
	        {
	        	in.unget();
	        	return lexNumber();
	        }
	        else if(isalpha(ch))
	        {
	        	in.unget();
	        	return lexVariableOrKeyword();
	        }
	        else if(ch == '\"')
	        {
	        	return lexString();
	        }
	        else if(in.eof())
	        {
	        	return new lexeme("ENDofINPUT");
	        }
	        else{
	        	// cout << "ch : " << ch << endl;
	        	return new lexeme("UNKNOWN", ch);
	        }
        }
}


lexeme* lexNumber()
{
	char ch;

	string number = "";
	in.get(ch);
	while(isdigit(ch))
	{
		number = number + ch;
		in.get(ch);
	}

	in.unget();
	return new lexeme("NUMBER", stoi(number));

}


lexeme* lexString(){

	char ch;
	string str = "";
	in.get(ch);

	while(ch != '\"')
	{
		str = str + ch;
		in.get(ch);
	}

	return new lexeme("STRING", str);
	
}


lexeme* lexVariableOrKeyword()
{
	char ch;
	string token = "";
	in.get(ch);
	while(isalpha(ch))
	{
		token = token + ch;
		in.get(ch);
	}

	in.unget();

	if(token.compare("array") == 0) return new lexeme("ARRAY");
	if(token.compare("else") == 0) return new lexeme("ELSE");
	if(token.compare("function") == 0) return new lexeme("FUNCTION");
	if(token.compare("if") == 0) return new lexeme("IF");
	if(token.compare("var") == 0) return new lexeme("VAR");
	if(token.compare("void") == 0) return new lexeme("VOID");
	if(token.compare("while") == 0) return new lexeme("WHILE");
	else return new lexeme("ID", token);
}


void skipWhiteSpace()
{
	char ch;
	in.get(ch);
	while(isspace(ch) && !in.eof())
	{
		if(ch == '\n') 
			line++;
		in.get(ch);
	}
	
	if(!in.eof()) 
		in.unget();
}