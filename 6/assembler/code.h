#ifndef HACK_CODE_H
#define HACK_CODE_H

#include "parser.h"

class Code{
private:
	std::string filename;
	Parser parser;
	std::shared_ptr<SymbolTable> symtab;
public:
	Code(const std::string& fn,std::shared_ptr<SymbolTable> symtab);
	
	std::string CodeDest(const std::string& dest);
	
	std::string CodeComp(const std::string& comp);
	
	std::string CodeJump(const std::string& jump);
	
	std::string CodeCombi();
	
	std::string CodeAInstr();
	
	void PrintSingleBinCode(const std::string& filename);
	
	void PrintBinCode(const std::string& filename);
	
	//Parser getParser();

};



#endif
