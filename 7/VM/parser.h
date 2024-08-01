#ifndef VM_PARSER_H
#define VM_PARSER_H

#include <string>
#include <fstream>
#include <cctype>
#include <vector>
#include <iostream>
#include <utility>

class Parser{
private:
	enum CmdType {
    		C_ARITHMETIC = -1,
    		C_PUSH = -2,
    		C_POP = -3,
    		C_LABEL = -4,
    		C_GOTO = -5,
    		C_IF = -6,
    		C_FUNCTION = -7,
    		C_RETURN = -8,
    		C_CALL = -9,
    		C_INVALID = 0
	};
	std::ifstream file;
	std::string filename;
    	char curChar;
    	void nextChar();
    	std::string curCmd;
    	int linenum;
    	
    	void skipComment();
public:
	Parser(const std::string& filename);

  	~Parser();
  	std::string getCmd();
  	

  	std::string getcurCmd() const;
	char getcurChar() const;
	int getlinenum() const;
	
	bool isSubStr(const std::string& str,const std::string& substr);
	bool isNumber(const std::string& str);
	std::pair<size_t,size_t> findspace(const std::string& str);
	
	bool hasMoreLines();
	void advance();
	int commandType();
	std::string arg1();
	int arg2();
	
	void PrintCmds(const std::string& filename);
	
};







#endif
