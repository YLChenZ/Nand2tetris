#ifndef HACK_SYMBOL_TABLE_H
#define HACK_SYMBOL_TABLE_H

#include <map>
#include <string>

class SymbolTable{
private:
	std::map<std::string,int> SymTab;
public:
	SymbolTable();
	
	void addEntry(const std::string& symbol,int address);
	
	bool contains(const std::string& symbol);
	
	int getAddress(const std::string& symbol);
	
	std::map<std::string,int> getSymTab() const;
};

#endif
