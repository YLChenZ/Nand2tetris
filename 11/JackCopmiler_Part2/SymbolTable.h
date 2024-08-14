#ifndef JACK_COMPILER_SYMBOL_TABLE_H
#define JACK_COMPILER_SYMBOL_TABLE_H


#include <string>
#include <map>
#include <iostream>
//#include <tuple>

enum class Kind{
	STATIC,
	FIELD,
	ARG,
	VAR,
	NONE
};

struct Value{
	std::string type;
	Kind kind;
	int index;
};

class SymTab{
	int StaticIndex;
	int FieldIndex;
	int ArgIndex;
	int VarIndex;
	
	std::map<std::string,Value> STMap;
public:
	SymTab();
	//~SyTab();
	void reset();
	void add(const std::string& name,const std::string& type,Kind kind);
	int varCount(Kind kind);
	Kind kindOf(const std::string& name);
	std::string typeOf(const std::string& name);
	int indexOf(const std::string& name);
	std::map<std::string,Value> getSTMap();
};



#endif
