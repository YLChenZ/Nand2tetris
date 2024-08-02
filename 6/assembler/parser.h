#ifndef HACK_PARSER_H
#define HACK_PARSER_H

#include <string>
#include <fstream>
#include <cctype>
#include <vector>
#include <iostream>
#include <memory>
#include "symboltable.h"

class Parser{
private:
	enum InstructionType {
    		A_INSTRUCTION = -1,
    		C_INSTRUCTION = -2,
    		L_INSTRUCTION = -3,
    		INVALID_INSTRUCTION = 0
	};
	std::ifstream file;
	std::ofstream outFilep;
	std::ofstream outFiles;
	std::string filename;
	std::string passn;
	std::string stn;
    	char curChar;
    	void nextChar();
    	std::string curInstr;
    	int linenum;
    	int varaddr;
    	int passflag;
    	std::shared_ptr<SymbolTable> symtab;
    	void skipComment();
public:
	Parser(const std::string& filename,int pf,std::shared_ptr<SymbolTable> symtab);
	Parser(const std::string& filename,std::shared_ptr<SymbolTable> symtab);
	//Parser(const Parser& other);
  	~Parser();
  	std::string getInstr();
  	
	void advance();
  	std::vector<std::string> getInstrVec();
  	void PrintSingleInstr();
  	void PrintInstrs();
  	void PrintSymTab();
  	
  	std::string getcurInstr() const;
	char getcurChar() const;
	std::shared_ptr<SymbolTable> getsymtab();
	int getlinenum() const;
	bool isNumber(const std::string& str);
	
	bool hasMoreLines();
	int instructionType();
	std::string symbol();
	std::string dest();
	std::string comp();
	std::string jump();
	
};







#endif
