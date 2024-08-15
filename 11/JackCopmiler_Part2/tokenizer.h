#ifndef JACK_COMPILER_TOKENIZER
#define JACK_COMPILER_TOKENIZER

#include <fstream>
#include <iostream>
#include <cctype>
#include "token.h"
#include <map>


enum KWD{
	CLASS,
	METHOD,
	FUNCTION,
	CONSTRUCTOR,
	INT,
	BOOLEAN,
	CHAR,
	VOID,
	VAR,
	STATIC,
	FIELD,
	LET,
	DO,
	IF,
	ELSE,
	WHILE,
	RETURN,
	KTRUE,
	KFALSE,
	KNULL,
	THIS,
	UNKNOWN
};
class Tokenizer{
private:
	std::string filename;
	std::ifstream file;
	std::ofstream outfile;
	char curChar;
	Token curToken;
	
	std::map<std::string,KWD> KWDMap;
	
	Token getID();
	Token getNum();
	Token getStr();
	bool isSymbol(char c);
public:
	Tokenizer(const std::string& filename);
	~Tokenizer();
	void nextChar();
	void skipComment();
	void skipSpace();
	Token getToken();
	bool hasMoreTokens();
	void advance();
	
	TokType tokenType();
	KWD keyWord();
	char symbol();
	std::string identifier();
	int intVal();
	std::string stringVal();
	
	Token getCurToken();
	
	void printSingleToken();
	
	void printTokens();
};



#endif
