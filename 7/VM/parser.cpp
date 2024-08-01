#include "parser.h"

void Parser::nextChar() {
	curChar = file.get();
}


Parser::Parser(const std::string& filename)
	: filename(filename), curChar(' '), curCmd(" "), linenum(-1)
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




std::string Parser::getCmd() {
	while (std::isspace(curChar)) {
       	nextChar();
	}
	
        while (curChar == '/') {
        	skipComment();
        	while (std::isspace(curChar)) {
            		nextChar();
        	}
    	}
	
	std::string Cmd;
	
	while (curChar != '\n' && curChar != '\r' && curChar != EOF){
		//std::cout << curChar<<'\n';
		Cmd += curChar;
		nextChar();
	}
	
	
	linenum ++;
	
	nextChar();
	
	return Cmd;
}

bool Parser::hasMoreLines(){
	return curChar != EOF;
}

void Parser::advance(){
	if (hasMoreLines()) {
        	curCmd = getCmd();
    	}
	//std::cout <<curCmd<<'\n';
}


std::string Parser::getcurCmd() const{
	return curCmd;
}

char Parser::getcurChar() const{
	return curChar;
}

int Parser::getlinenum() const{
	return linenum;
}


bool Parser::isSubStr(const std::string& str,const std::string& substr){
	return str.find(substr) != std::string::npos;
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

std::pair<size_t,size_t> Parser::findspace(const std::string& str){
	std::pair<size_t,size_t> goodres(0,0);
	std::pair<size_t,size_t> badres(0,0);
	
	size_t first_space = str.find(' ');
	size_t second_space = str.find(' ', first_space + 1);
	
    	if (first_space != std::string::npos && second_space != std::string::npos) {
        	goodres.first = first_space;
        	goodres.second = second_space;
    	} else {
        	return badres;
    	}

    	return goodres;
}

int Parser::commandType(){
	//advance();
	
	if (curCmd == "add" || curCmd == "sub" || curCmd == "neg"
		|| curCmd == "eq" || curCmd == "gt" || curCmd == "lt" 
		|| curCmd == "and" || curCmd == "or" || curCmd == "not")
		return C_ARITHMETIC;
	if (isSubStr(curCmd,"push"))
		return C_PUSH;
	if (isSubStr(curCmd,"pop"))
		return C_POP;
	return 0;
}


std::string Parser::arg1(){
	int C_Type = commandType();
	if (C_Type == C_ARITHMETIC)
		return curCmd;
	if (C_Type == C_PUSH || C_Type == C_POP){
		auto space = findspace(curCmd);
		return curCmd.substr(space.first+1,space.second - space.first - 1);
	}
	return "None";
}

int Parser::arg2(){
	int C_Type = commandType();
	if (C_Type == C_ARITHMETIC)
		return 0;
	if (C_Type == C_PUSH || C_Type == C_POP){
		auto space = findspace(curCmd);
		auto strres = curCmd.substr(space.second+1);
		return std::stoi(strres);
	}
	return -1;
}


void Parser::PrintCmds(const std::string& filename){
	std::ofstream outFile(filename);
    	if (!outFile.is_open()) {
        	std::cerr << "Error opening file: " << filename << std::endl;
        	return;
    	}	
	
	while (curChar != EOF){
		outFile << linenum << ": " << curCmd << " " 
			<< "  arg1: " << arg1() << "  arg2: " << arg2() << '\n';
		advance();
	}
	
	outFile.close();
}



