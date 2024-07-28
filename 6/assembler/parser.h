#ifndef HACK_PARSER_H
#define HACK_PARSER_H

#include <string>
#include <fstream>
#include <cctype>
#include <vector>
#include <iostream>

class Parser{
private:
	enum InstructionType {
    		A_INSTRUCTION = -1,
    		C_INSTRUCTION = -2,
    		L_INSTRUCTION = -3,
    		INVALID_INSTRUCTION = 0
	};
	std::ifstream file;
    	char curChar;
    	void nextChar();
    	std::string curInstr;
    	
    	void skipComment();
public:
	Parser(const std::string& filename);
  	~Parser();
  	std::string getInstr();
  	
	void advance();
  	std::vector<std::string> getInstrVec();
  	void PrintSingleInstr();
  	void PrintInstrs();
  	
  	std::string getcurInstr() const;
	char getcurChar() const;
	bool hasMoreLines();


	int instructionType();
	std::string symbol();
	std::string dest();
	std::string comp();
	std::string jump();
	

};







#endif
