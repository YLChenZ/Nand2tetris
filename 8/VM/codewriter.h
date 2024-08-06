#ifndef VM_CODE_WRITER_H
#define VM_CODE_WRITER_H

#include "parser.h"
#include <filesystem>

class CodeWriter{
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
	std::ofstream& outFile;
	std::string filename;
	
	//std::filesystem::path dirPath;
	//std::string outfn;
	std::string curFunName;
	int compIndex;
	int callIndex;
	Parser parser;
public:
	//CodeWriter(const std::string& filename);
	
	CodeWriter(const std::string& filename, std::ofstream& outFile);
	
	~CodeWriter();
	
	void D2SP();
	void SP2D();

	void writeArithmetic(const std::string& command);
	
	void writePushPop(int ct, const std::string& segment, int index);
	
	void writeLabel(const std::string& label);
	
	void writeGoto(const std::string& label);
	
	void writeIf(const std::string& label);
	
	void writeFunction(const std::string& funName,int nVars);
	
	void writeCall(const std::string& funName,int nArgs);
	
	void writeReturn();
	
	void SingleVm2Asm();
	
	void Vm2Asm();
};


#endif
