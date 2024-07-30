#include "parser.h"

void Parser::nextChar() {
	curChar = file.get();
}


Parser::Parser(const std::string& filename,int pf,std::shared_ptr<SymbolTable> symtab) 
	: filename(filename), curChar(' '), curInstr(" "), linenum(-1), varaddr(15), passflag(pf), symtab(symtab)
{
	file.open(filename);
	if (!file.is_open()) {
		std::cerr << "Error opening file" << '\n';
		exit(1);
	}
	nextChar();
	advance();
}

Parser::Parser(const std::string& filename,std::shared_ptr<SymbolTable> symtab)
	: filename(filename), curChar(' '), curInstr(" "), linenum(-1), varaddr(15), symtab(symtab)
{
	file.open(filename);
	if (!file.is_open()) {
		std::cerr << "Error opening file" << '\n';
		exit(1);
	}
	nextChar();
	advance();
}



Parser::~Parser() {
	if (file.is_open()) {
		file.close();
	}
}


void Parser::skipComment() {
    if (curChar == '/') {
        nextChar();
        if (curChar == '/') {
            while (curChar != '\n' && curChar != EOF) {
                nextChar();
            }
            if (curChar == '\n') {
                nextChar();
            }
        } 
    }
}




std::string Parser::getInstr() {
	while (std::isspace(curChar)) {
       	nextChar();
	}
	
        while (curChar == '/') {
        	skipComment();
        	while (std::isspace(curChar)) {
            		nextChar();
        	}
    	}
	
	std::string Instr;
	
	while (curChar != '\n' && curChar != '\r' && !isspace(curChar) && curChar != EOF){
		//std::cout << curChar<<'\n';
		Instr += curChar;
		nextChar();
	}
	
	if (Instr.find('(') == std::string::npos)
		linenum ++;
	
	nextChar();
	
	return Instr;
}

bool Parser::hasMoreLines(){
	return curChar != EOF;
}

void Parser::advance(){
	if (hasMoreLines()) {
        	curInstr = getInstr();
    	}
	//std::cout <<curInstr<<'\n';
}


std::string Parser::getcurInstr() const{
	return curInstr;
}

char Parser::getcurChar() const{
	return curChar;
}

int Parser::getlinenum() const{
	return linenum;
}

std::shared_ptr<SymbolTable> Parser::getsymtab() {
	return symtab;
}

bool Parser::isNumber(const std::string& str) {
    try {
        std::stoi(str);
        return true;
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
}

int Parser::instructionType(){
	//advance();
	
	if (curInstr[0] == '@')
		return A_INSTRUCTION;
	if (curInstr[0] == '(')
		return L_INSTRUCTION; 
	if (curInstr.find(';') != std::string::npos || curInstr.find('=') != std::string::npos )
		return C_INSTRUCTION;
	return 0;
}


std::string Parser::symbol(){
	int instType = instructionType();
	std::string symbol;
	
	if (passflag == 2 && instType == -1){
		symbol = curInstr.substr(1);
		if (!symtab->contains(symbol) && !isNumber(symbol)){
			varaddr++;
			symtab->addEntry(symbol,varaddr);
			return symbol;
		}
	}
	
	if (passflag == 1 && instType == -3){
		symbol = curInstr.substr(1, curInstr.length() - 2);
		if (!symtab->contains(symbol)){
			symtab->addEntry(symbol,linenum+1);
			return symbol;
		}		
	}
	
	if (passflag != 1 && passflag != 2 && instType == -1){
		symbol = curInstr.substr(1);
		if (!symtab->contains(symbol) && !isNumber(symbol)){
			varaddr++;
			symtab->addEntry(symbol,varaddr);
		}
	}
	if (passflag != 1 && passflag != 2 && instType == -3){
		symbol = curInstr.substr(1, curInstr.length() - 2);
		if (!symtab->contains(symbol))
			symtab->addEntry(symbol,linenum+1);
	}
	
	//std::cout<< varaddr;
	
	return symbol;
}


std::string Parser::dest(){
	std::string dest;
	if (instructionType() == -2){
		if (curInstr.find(';') != std::string::npos)
			dest = "None";
		else {
			size_t eqindex = curInstr.find('=');
			dest = curInstr.substr(0,eqindex);
		}	
	}
	return dest;
}
std::string Parser::comp(){
	std::string comp;
	if (instructionType() == -2){
		size_t semiindex = curInstr.find(';');
		size_t eqindex = curInstr.find('=');
		if (semiindex != std::string::npos)
			comp = curInstr.substr(0,semiindex);
		else if (eqindex != std::string::npos){
			comp = curInstr.substr(eqindex+1);
		}	
	}
	return comp;
}
std::string Parser::jump(){
	std::string jump;
	if (instructionType() == -2){
		size_t semiindex = curInstr.find(';');
		size_t eqindex = curInstr.find('=');
		if (semiindex != std::string::npos)
			jump = curInstr.substr(semiindex+1);
		else if (eqindex != std::string::npos){
			jump = "None";
		}	
	}
	return jump;
}

std::vector<std::string> Parser::getInstrVec()
{
	std::vector<std::string> InstrVec;
	
	std::string ins = curInstr;
	
	while (curChar != EOF) {
		InstrVec.push_back(ins);
		ins = getInstr();
	} 
	return InstrVec;
}


      
void Parser::PrintSingleInstr(const std::string& filename){
	std::ofstream outFile(filename,std::ios::app);
    	if (!outFile.is_open()) {
        	std::cerr << "Error opening file: " << filename << std::endl;
        	return;
    	}
    	
	int instType = instructionType();
	if (instType == -1 || instType == -3){
	outFile << linenum << " : " << curInstr << curInstr.size() << "   symbol: " << symbol() << '\n';
	}
	
	if (instructionType() == -2){
	outFile << linenum << " : " << curInstr << curInstr.size() << "  dest: " << dest() 
			      << " comp: " << comp()
			      << " jump: " << jump() <<'\n';
	}
	outFile.close();
}      
void Parser::PrintInstrs(const std::string& filename)
{
	while (curChar != EOF){
		PrintSingleInstr(filename);
		advance();
	}
}

void Parser::PrintSymTab(const std::string& filename)
{
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    
    for (const auto& kv : symtab->getSymTab()){
        outFile << "Key: " << kv.first << " Value: " << kv.second << '\n';
    }

    outFile.close();
}

