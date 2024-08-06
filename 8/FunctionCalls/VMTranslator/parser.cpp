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
            while (curChar != '\n' && curChar != '\r' && curChar != EOF) {
                nextChar();
            }
            if (curChar == '\n' || curChar == '\r') {
                nextChar();
            }
        } 
    }
}




std::string Parser::getCmd() {
    std::string Cmd;

    while (isspace(curChar) || curChar == '/') {
        if (curChar == '/') {
            skipComment();
        } else {
            nextChar();
        }
    }

    int spacecount = 0;

    while (curChar != '\n' && curChar != '\r' && curChar != EOF) {
        if (curChar == '/') {
            skipComment();
            continue;
        }
        if (isspace(curChar)) {
            spacecount++;
        } else {
            spacecount = 0;
        }

        if (spacecount < 3) {
            Cmd += curChar;
        }

        nextChar();
    }

    Cmd.erase(Cmd.find_last_not_of(" \t") + 1);

    linenum++;

    
    if (curChar == '\n' || curChar == '\r') {
        nextChar();
    }
    
    if (Cmd.empty() && curChar == EOF) {
        Cmd = "EOF";
    }
    // 调试输出
    std::cout << "Read command: \"" << Cmd << "\"" << std::endl;

    return Cmd;
}

bool Parser::hasMoreLines(){
	return curChar != EOF;
}

void Parser::advance() {
    if (hasMoreLines()) {
        curCmd = getCmd();
    } else {
        curCmd.clear(); // Clear the current command when no more lines
    }
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
	if (isSubStr(curCmd,"label"))
		return C_LABEL;
	if (isSubStr(curCmd,"goto") && !isSubStr(curCmd,"if"))
		return C_GOTO;
	if (isSubStr(curCmd,"if-goto"))
		return C_IF;
	if (isSubStr(curCmd,"function"))
		return C_FUNCTION;
	if (curCmd == "return")
		return C_RETURN;
	if (isSubStr(curCmd,"call"))
		return C_CALL;
	return C_INVALID;
}


std::string Parser::arg1(){
	int C_Type = commandType();
	auto space = findspace(curCmd);
	size_t first_space = curCmd.find(' ');
	switch (C_Type){
		case -2: case -3: case -7: case -9:
			return curCmd.substr(space.first+1,space.second - space.first - 1);
			break;
		
		case -1:
			return curCmd;
			break;
			
		case -4: case -5: case -6:
			if (first_space!= std::string::npos)
				return curCmd.substr(first_space+1);
			break;
		default:
			return "None";		
	}
}

int Parser::arg2(){
	int C_Type = commandType();
	auto space = findspace(curCmd);

	switch (C_Type){
		case -2: case -3: case -7: case -9:
			return std::stoi(curCmd.substr(space.second+1));
			break;
		default:
			return -1;
			
	}

}


void Parser::PrintCmds(const std::string& filename){
	std::ofstream outFile(filename);
    	if (!outFile.is_open()) {
        	std::cerr << "Error opening file: " << filename << std::endl;
        	return;
    	}	
	
	do {
        outFile << linenum << ": " << curCmd << " "
                << "arg1: " << arg1() << " arg2: " << arg2() << '\n';
        advance();
    	} while (hasMoreLines() || !curCmd.empty());
	
	outFile.close();
}



