#include "code.h"
#include <bitset>

Code::Code(const std::string& fn,std::shared_ptr<SymbolTable> symtab) 
	: filename(fn), parser(Parser(fn,symtab)), symtab(symtab) {}

std::string Code::CodeDest(const std::string& dest){
	if (dest == "None")
		return "000";
	else if (dest == "M")
		return "001";
	else if (dest == "D")
		return "010";
	else if (dest == "DM" || dest == "MD")
		return "011";
	else if (dest == "A")
		return "100";
	else if (dest == "AM")
		return "101";
	else if (dest == "AD")
		return "110";
	else if (dest == "ADM")
		return "111";
}
	
std::string Code::CodeComp(const std::string& comp){
	std::string compLSB = "0";
	std::string compRight = "000000";
	
	if (comp == "0")
		compRight = "101010";
	else if (comp == "1")
		compRight = "111111";
	else if (comp == "-1")
		compRight = "111010";
	else if (comp == "D")
		compRight = "001100";
	else if (comp == "A" || comp == "M")
		compRight = "110000";
	else if (comp == "!D")
		compRight = "001101";
	else if (comp == "!A" || comp == "!M")
		compRight = "110001";
	else if (comp == "-D")
		compRight = "001111";
	else if (comp == "-A" || comp == "-M")
		compRight = "110011";
	else if (comp == "D+1")
		compRight = "011111";
	else if (comp == "A+1" || comp == "M+1")
		compRight = "110111";
	else if (comp == "D-1")
		compRight = "001110";
	else if (comp == "A-1" || comp == "M-1")
		compRight = "110010";
	else if (comp == "D+A" || comp == "D+M")
		compRight = "000010";
	else if (comp == "D-A" || comp == "D-M")
		compRight = "010011";
	else if (comp == "A-D" || comp == "M-D")
		compRight = "110001";
	else if (comp == "D&A" || comp == "D&M")
		compRight = "000000";
	else if (comp == "D|A" || comp == "D|M")
		compRight = "010101";
	if (comp.find('M') != std::string::npos)
		compLSB="1";
	else if (comp.find('A') != std::string::npos)
		compLSB="0";
		
	return compLSB + compRight;
}
	
std::string Code::CodeJump(const std::string& jump){
	if (jump == "None")
		return "000";
	else if (jump == "JGT")
		return "001";
	else if (jump == "JEQ")
		return "010";
	else if (jump == "JGE")
		return "011";
	else if (jump == "JLT")
		return "100";
	else if (jump == "JNE")
		return "101";
	else if (jump == "JLE")
		return "110";
	else if (jump == "JMP")
		return "111";
}

std::string Code::CodeCombi(){
	auto DestBin = CodeDest(parser.dest());
	//std::cout << DestBin <<'\n';
	auto CompBin = CodeComp(parser.comp());
	//std::cout << CompBin <<'\n';
	auto JumpBin = CodeJump(parser.jump());
	//std::cout << parser.jump();
	//std::cout << parser.jump().size();
	//std::cout << JumpBin <<'\n';
	
	std::string result = "111";
	
	result = result + CompBin + DestBin + JumpBin;
	
	return result;
}

std::string Code::CodeAInstr(){
	auto symbol = parser.symbol();
	int DNumber;
	if (parser.isNumber(symbol))
		DNumber = std::stoi(symbol);
	else 
		DNumber = parser.getsymtab()->getSymTab()[symbol];
		
	std::bitset<15> BNumber(DNumber);
	auto AddrBin = BNumber.to_string(); 
	return "0" + AddrBin;
}

void Code::PrintSingleBinCode(const std::string& filename) {
	std::ofstream outFile(filename,std::ios::app);
    	if (!outFile.is_open()) {
        	std::cerr << "Error opening file: " << filename << std::endl;
        	return;
    	}
    	
	int instType = parser.instructionType();
	if (instType == -1){
		outFile << CodeAInstr() << '\n';
	}
	
	if (instType == -2){
		outFile << CodeCombi() << '\n';
	}
}

void Code::PrintBinCode(const std::string& filename)
{	
	
	while (parser.getcurChar()!=EOF){
		PrintSingleBinCode(filename);
		parser.advance();
	}
}

//Parser Code::getParser(){
////	return parser;
//}

