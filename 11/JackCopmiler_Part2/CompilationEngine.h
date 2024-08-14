#ifndef JACK_COMPILER_COMPILATION_ENGINE
#define JACK_COMPILER_COMPILATION_ENGINE


#include "tokenizer.h"
#include "VMWriter.h"
#include "SymbolTable.h"

class CompilationEngine{
private:
	//std::string filename;
	//std::ifstream in;
	//std::ofstream out;
	//Token CurTok;
	Tokenizer tok;
	VMWriter vmw;
	SymTab ClassLevelSt;
	SymTab SubroutineLevelSt;
	std::string classname;
	std::string funname;
	KWD FuncKind;
	int IfLabelIndex;
	int WhileLabelIndex;

public:
	CompilationEngine(const std::string& filename);
	//~CompilationEngine();
	
	//void getNextToken();
	bool isDataType();
	bool isRetType();
	bool isOp();
	bool isunaryOp();
	bool isKwdConst();
	Value findInST();
	
	void compileClass();
	void compileClassVarDec();
	void compileSubroutine();
	void compileParameterList();
	void compileSubroutineBody();
	void compileVarDec();
	void compileStatements();
	void compileLet();
	void compileIf();
	void compileWhile();
	void compileDo();
	void compileReturn();
	void compileExpression();
	void compileTerm();
	int compileExpressionList();
	
	void printVMCode();
};


#endif
