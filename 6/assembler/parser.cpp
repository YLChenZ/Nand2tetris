#include "parser.h"

void Parser::nextChar() {
	curChar = file.get();
}


Parser::Parser(const std::string& filename) : curChar(' '), curInstr(" ") {
	file.open(filename);
	if (!file.is_open()) {
		std::cerr << "Error opening file" << std::endl;
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
	
	while (curChar != '\n' && curChar != '\r' && curChar != EOF){
		//std::cout << curChar<<'\n';
		Instr += curChar;
		nextChar();
	}
	
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
	
	if (instType == -1){
		symbol = curInstr.substr(1);
	}
	
	if (instType == -3){
		symbol = curInstr.substr(1, curInstr.length() - 2);
	}
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


      
void Parser::PrintSingleInstr(){
	int instType = instructionType();
	if (instType == -1 || instType == -3){
	std::cout << curInstr << "   symbol: " << symbol() << '\n';
	}
	
	if (instructionType() == -2){
	std::cout << curInstr << "  dest: " << dest() 
			       << " comp: " << comp()
			       << " jump: " << jump() <<'\n';
	}
}      
void Parser::PrintInstrs()
{
	while (curChar != EOF){
		PrintSingleInstr();
		advance();
	}
}

