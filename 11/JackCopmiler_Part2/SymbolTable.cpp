#include "SymbolTable.h"

SymTab::SymTab()
	: StaticIndex(0), FieldIndex(0), ArgIndex(0), VarIndex(0), STMap({})
{

}


void SymTab::reset(){
	STMap.clear();
	StaticIndex = 0;
	FieldIndex = 0;
	ArgIndex = 0;
	VarIndex = 0;
}

std::map<Kind,std::string> Kind2Str{{Kind::STATIC,"static"},{Kind::FIELD,"field"},{Kind::ARG,"arg"},{Kind::VAR,"var"},
{Kind::NONE,"none"}};

void SymTab::add(const std::string& name,const std::string& type,Kind kind){
	Value v = {type,kind,0};
	if (kind == Kind::STATIC){
		v.index = StaticIndex;
		STMap.emplace(name,v);
		StaticIndex++;
	}
	if (kind == Kind::FIELD){
		v.index = FieldIndex;
		STMap.emplace(name,v);
		FieldIndex++;
	}
	if (kind == Kind::ARG){
		v.index = ArgIndex;
		STMap.emplace(name,v);
		ArgIndex++;
	}
	if (kind == Kind::VAR){
		v.index = VarIndex;
		STMap.emplace(name,v);
		VarIndex++;
	}
	//else std::cout << "Failed add "<<name<<" "<<type<<" "<<Kind2Str[kind]<<'\n';
	
}


int SymTab::varCount(Kind kind){
	if (kind == Kind::STATIC)
		return StaticIndex;
	if (kind == Kind::FIELD)
		return FieldIndex;
	if (kind == Kind::ARG)
		return ArgIndex;
	if (kind == Kind::VAR)
		return VarIndex;
	else return -1;
}


Kind SymTab::kindOf(const std::string& name){
	return STMap[name].kind;
}


std::string SymTab::typeOf(const std::string& name){
	return STMap[name].type;
}


int SymTab::indexOf(const std::string& name){
	return STMap[name].index;
}

std::map<std::string,Value> SymTab::getSTMap(){
	return STMap;
}
