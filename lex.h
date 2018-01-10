#ifndef _lex_h
#define _lex_h

#include <string>
#include <vector>
#include <fstream>

using namespace std;

class lexeme;

class lexeme
{
	public:

		//constructors
		lexeme(string t)
		{
			type = t; 
			right = NULL;
			left = NULL;
			v = new vector<int>();
		}

		lexeme(string t, string s)
		{
			type = t;
			val = s;
			right = NULL;
			left = NULL;
			v = new vector<int>();
		}

		lexeme(string t, int num){
			type = t;
			i = num;
			right = NULL;
			left = NULL;
			v = new vector<int>();
		}

		lexeme(string t, lexeme* variables, lexeme* values) //much like creating a new cons 
		{
			type = t;
			left = variables;
			right = values;
			v = new vector<int>();
		}

		//getters
		string getType()
		{
			return type;
		}

		string getValue()
		{
			return val;
		}

		vector<int>* getVector()
		{
			return v;
		}

		int getInt(){
			return i;
		}

		lexeme* getLeft()
		{
			return left;
		}

		lexeme* getRight()
		{
			return right;
		}

		//setters
		void setInt(int num){
			i = num;
		}

		void setLeft(lexeme *L){
			left = L;
		}

		void setRight(lexeme *R){
			right = R;
		}

		//combining operations to create trees

		void cons(lexeme *L, lexeme *R)
		{
			left = L;
			right = R;
		}

	private:
		string type, val;
		vector<int> *v;
		int i;
		lexeme *left, *right;

};

lexeme* lex();

lexeme* lexNumber();

lexeme* lexString();

lexeme* lexVariableOrKeyword();

void skipWhiteSpace();

#endif
