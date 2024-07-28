#include "code.h"
#include <bitset>

Code::Code(const std::string& fn) : filename(fn), parser(Parser(fn)) {}

std::string Code::CodeDest(const std::string& dest){
	if (dest == "None")
		return "000";
	if (dest == "M")
		return "001";
	if (dest == "D")
		return "010";
	if (dest == "DM")
		return "011";
	if (dest == "A")
		return "100";
	if (dest == "AM")
		return "101";
	if (dest == "AD")
		return "110";
	if (dest == "ADM")
		return "111";
}
	
std::string Code::CodeComp(const std::string& comp){
	std::string compLSB = "0";
	std::string compRight = "000000";
	
	if (comp == "0")
		compRight = "101010";
	if (comp == "1")
		compRight = "111111";
	if (comp == "-1")
		compRight = "111010";
	if (comp == "D")
		compRight = "001100";
	if (comp == "A" || comp == "M")
		compRight = "110000";
	if (comp == "!D")
		compRight = "001101";
	if (comp == "!A" || comp == "!M")
		compRight = "110001";
	if (comp == "-D")
		compRight = "001101";
	if (comp == "!A" || comp == "!M")
		compRight = "110001";
	if (comp == "-D")
		compRight = "001111";
	if (comp == "-A" || comp == "-M")
		compRight = "110011";
	if (comp == "D+1")
		compRight = "011111";
	if (comp == "A+1" || comp == "M+1")
		compRight = "110111";
	if (comp == "D-1")
		compRight = "001110";
	if (comp == "A-1" || comp == "M-1")
		compRight = "110010";
	if (comp == "D+A" || comp == "D+M")
		compRight = "000010";
	if (comp == "D-A" || comp == "D-M")
		compRight = "010011";
	if (comp == "A-D" || comp == "M-D")
		compRight = "110001";
	if (comp == "D&A" || comp == "D&M")
		compRight = "000000";
	if (comp == "D|A" || comp == "D|M")
		compRight = "010101";
	if (parser.getcurInstr().find('M') != std::string::npos)
		compLSB="1";
	else if (parser.getcurInstr().find('A') != std::string::npos)
		compLSB="0";
		
	return compLSB + compRight;
}
	
std::string Code::CodeJump(const std::string& jump){
	if (jump == "None")
		return "000";
	if (jump == "JGT")
		return "001";
	if (jump == "JEQ")
		return "010";
	if (jump == "JGE")
		return "011";
	if (jump == "JLT")
		return "100";
	if (jump == "JNE")
		return "101";
	if (jump == "JLE")
		return "110";
	if (jump == "JMP")
		return "111";
}

std::string Code::CodeCombi(){
	auto DestBin = CodeDest(parser.dest());
	auto CompBin = CodeComp(parser.comp());
	auto JumpBin = CodeJump(parser.jump());
	
	return "111" + CompBin + DestBin + JumpBin;
}

std::string Code::CodeAInstr(){
	auto symbol = parser.symbol();
	int DNumber = std::stoi(symbol);
	std::bitset<15> BNumber(DNumber);
	auto AddrBin = BNumber.to_string(); 
	return "0" + AddrBin;
}

void Code::PrintSingleBinCode() {
	int instType = parser.instructionType();
	if (instType == -1 || instType == -3){
		std::cout << CodeAInstr() << '\n';
	}
	
	if (instType == -2){
		std::cout << CodeCombi() << '\n';
	}
}

void Code::PrintBinCode()
{
	while (parser.getcurChar() != EOF){
		PrintSingleBinCode();
		parser.advance();
	}
}
