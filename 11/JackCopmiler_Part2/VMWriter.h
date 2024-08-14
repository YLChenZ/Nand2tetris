#ifndef JACK_COMPILER_VMWRITER_H
#define JACK_COMPILER_VMWRITER_H

#include <string>
#include <fstream>
#include <map>
#include <iostream>

enum class Segment{
	CONSTANT,
	ARGUMENT,
	LOCAL,
	STATIC,
	THIS,
	THAT,
	POINTER,
	TEMP
};

enum class Command{
	ADD,
	SUB,
	NEG,
	EQ,
	GT,
	LT,
	AND,
	OR,
	NOT
};




class VMWriter{
private:
	std::ofstream outfs;

public:
	VMWriter(const std::string& filename);
	~VMWriter();
	void writePush(Segment seg,int index);
	void writePop(Segment seg,int index);
	void writeArithmetic(Command cmd);
	void writeLabel(const std::string& label);
	void writeGoto(const std::string& label);
	void writeIfGoto(const std::string& label);
	void writeCall(const std::string& name,int nArgs);
	void writeFunction(const std::string& name,int nVars);
	void writeReturn();
	
};

#endif
