#include "codewriter.h"
#include <map>

CodeWriter::CodeWriter(const std::string& filename)
	: filename(filename), parser(Parser(filename)),compIndex(-1)
{
	file.open(filename);
	if (!file.is_open()) {
		std::cerr << "Error opening file" << '\n';
		exit(1);
	}
	
	size_t commapos = filename.find('.');
	std::string fnprefix;
	if (commapos != std::string::npos)
		fnprefix = filename.substr(0,commapos);
	outfn = fnprefix + ".asm";
	
	outFile.open(outfn);
    	if (!outFile.is_open()) {
        	std::cerr << "Error opening file: " << outfn << '\n';
        	exit(1);
    	}
    	
    	
}



CodeWriter::~CodeWriter() {
	if (file.is_open()) {
		file.close();
	}
	
	if (outFile.is_open()) {
		outFile.close();
	}
	
	
}


std::map<std::string,char> AL2OPMap = { {"add",'+'}, {"sub",'-'}, {"and",'&'}, {"or",'|'} };

std::map<std::string,int> Cmd2Int = {{"add",1}, {"sub",2}, {"neg",3}, {"eq",4}, {"gt",5}, {"lt",6}, {"and",7}, {"or",8}, {"not",9}};

std::map<std::string,char> AL1OPMap = { {"neg",'-'}, {"not",'!'}};

std::map<std::string,std::string> CompMap = { {"eq","EQ"}, {"gt","GT"}, {"lt","LT"}};


void CodeWriter::D2SP(){
	outFile << "@SP" << '\n'
		<<"A=M"<<'\n'
		<<"M=D"<<'\n'
		<< "@SP" << '\n' 
		<<"M=M+1"<<'\n';
}

void CodeWriter::SP2D(){
	outFile <<"@SP"<<'\n'
		<<"M=M-1"<<'\n'
		<<"A=M"<<'\n'
		<<"D=M"<<'\n';
}
void CodeWriter::writeArithmetic(const std::string& command){
	
	switch (Cmd2Int[command]){
	case 1: case 7: case 8:
		outFile << "//"<<command<<'\n';
		SP2D();
				/*<<"@SP"<<'\n'//rep2
				<<"M=M-1"<<'\n'
				<<"A=M"<<'\n'
				<<"D=M"<<'\n'*/
		outFile <<"@SP"<<'\n' 
				<<"M=M-1"<<'\n' 
				<<"A=M"<<'\n'
				<<"D=D"<<AL2OPMap[command]<<"M"<<'\n';
		D2SP();
				/*<< "@SP" << '\n' //rep1 
				<<"A=M"<<'\n'
				<<"M=D"<<'\n'
				<< "@SP" << '\n' 
				<<"M=M+1"<<'\n';*/
				
		break;
	case 2: 
		outFile << "//"<<command<<'\n';
		SP2D();
				/*<<"@SP"<<'\n'//rep2
				<<"M=M-1"<<'\n'
				<<"A=M"<<'\n'
				<<"D=M"<<'\n'*/
		outFile <<"@SP"<<'\n' 
				<<"M=M-1"<<'\n' 
				<<"A=M"<<'\n'
				<<"D=M-D"<<'\n';
		D2SP();		
				/*<< "@SP" << '\n' //rep1 
				<<"A=M"<<'\n'
				<<"M=D"<<'\n'
				<< "@SP" << '\n' 
				<<"M=M+1"<<'\n';*/
			break;
	case 3: case 9:
		outFile << "//"<<command<<'\n'
				<< "@SP" << '\n'
				<<"M=M-1"<<'\n'
				<<"A=M"<<'\n'
				<<"D="<<AL1OPMap[command]<<"M"<<'\n';
		D2SP();
				/*<< "@SP" << '\n' //rep1 
				<<"A=M"<<'\n'
				<<"M=D"<<'\n'
				<< "@SP" << '\n' 
				<<"M=M+1"<<'\n';*/
		break;
	
	case 4: case 5: case 6:
		compIndex++;
		outFile << "//"<<command<<'\n';
		SP2D();
				/*<< "@SP" << '\n' //rep2
				<<"M=M-1"<<'\n'
				<<"A=M"<<'\n'
				<<"D=M"<<'\n'*/
		outFile << "@SP" << '\n' 
				<<"M=M-1"<<'\n' 
				<<"A=M"<<'\n'
				<<"D=M-D"<<'\n'
				<<"@"<<CompMap[command]<<"_"<<compIndex<<'\n'
				<<"D;J"<<CompMap[command]<<'\n'
				<<"@SP"<<'\n'
				<<"A=M"<<'\n'
				<<"M=0"<<'\n'
				<<"@"<<CompMap[command]<<"_CONTINUE"<<compIndex<<'\n'
				<<"0;JMP"<<'\n'
				<<"("<<CompMap[command]<<"_"<<compIndex<<")"<<'\n'
				<<"@SP"<<'\n'
				<<"A=M"<<'\n'
				<<"M=-1"<<'\n'
				<<"("<<CompMap[command]<<"_CONTINUE"<<compIndex<<")"<<'\n'
				<<"@SP"<<'\n'
				<<"M=M+1"<<'\n';
		
		break;
	default:
		outFile<<"Bad translation in AL!"<<'\n';
	
	}
}


std::map<std::string,std::string> SegMap = {{"local","LCL"}, {"argument","ARG"}, {"this","THIS"}, {"that","THAT"}};

std::map<std::string,int> Seg2Int = {{"argument",1},{"local",2},{"static",3},{"constant",4}, {"this",5}, {"that",6},{"pointer",7},{"temp",8} };

void CodeWriter::writePushPop(int ct, const std::string& segment, int index){
	size_t commapos = filename.find('.');
	std::string fnprefix;
	if (commapos != std::string::npos)
		fnprefix = filename.substr(0,commapos+1);
		
	if (ct == C_PUSH){
		switch (Seg2Int[segment]){
		case 1: case 2: case 5: case 6:
			outFile << "//push "<<segment<<" "<<index<<'\n'
					<<"@"<<SegMap[segment]<<'\n'
					<<"D=M"<<'\n'
					<<"@"<<index<<'\n'
					<<"A=D+A"<<'\n'
					<<"D=M"<<'\n';
			D2SP();
					/*<< "@SP" << '\n' //rep1 
					<<"A=M"<<'\n'
					<<"M=D"<<'\n'
					<< "@SP" << '\n' 
					<<"M=M+1"<<'\n';*/
			break;
		case 3:
			outFile << "//push static "<<index<<'\n'
					<<"@"<<fnprefix<<index<<'\n'
					<<"D=M"<<'\n';
			D2SP();
					/*<< "@SP" << '\n' //rep1 
					<<"A=M"<<'\n'
					<<"M=D"<<'\n'
					<< "@SP" << '\n' 
					<<"M=M+1"<<'\n';*/
			break;
		case 4:
			outFile << "//push constant "<<index<<'\n'
					<<"@"<<index<<'\n'
					<<"D=A"<<'\n';
			D2SP();
					/*<< "@SP" << '\n' //rep1 
					<<"A=M"<<'\n'
					<<"M=D"<<'\n'
					<< "@SP" << '\n' 
					<<"M=M+1"<<'\n';*/
			break;
		case 7:
			outFile << "//push pointer "<<index<<'\n'
				<<"@"<<3 + index<<'\n'
				<<"D=M"<<'\n';
			D2SP();
				/*<< "@SP" << '\n' //rep1 
				<<"A=M"<<'\n'
				<<"M=D"<<'\n'
				<< "@SP" << '\n' 
				<<"M=M+1"<<'\n';*/
			break;
		case 8:
			outFile << "//push temp "<<index<<'\n'
					<<"@"<<5 + index<<'\n'
					<<"D=M"<<'\n';
			D2SP();
					/*<< "@SP" << '\n' //rep1 
					<<"A=M"<<'\n'
					<<"M=D"<<'\n'
					<< "@SP" << '\n' 
					<<"M=M+1"<<'\n';*/
			break;
		
		default:
			outFile<<"Bad translation!"<<'\n';
		}
	}	
	if (ct == C_POP){
		switch (Seg2Int[segment]){
		case 1: case 2: case 5: case 6:
			outFile << "//pop "<<segment<<" "<<index<<'\n'
					<<"@"<<SegMap[segment]<<'\n'
					<<"D=M"<<'\n'
					<<"@"<<index<<'\n'
					<<"D=D+A"<<'\n'
					<<"@R13"<<'\n'
					<<"M=D"<<'\n';
			SP2D();
					/*<< "@SP" << '\n' //rep2
					<<"M=M-1"<<'\n'
					<<"A=M"<<'\n'
					<<"D=M"<<'\n'*/
			outFile	<<"@R13"<<'\n'
					<<"A=M"<<'\n'
					<<"M=D"<<'\n';			
			break;
		case 3:
			outFile  << "//pop static "<<index<<'\n';
			SP2D();
					/*<< "@SP" << '\n' //rep2
					<<"M=M-1"<<'\n'
					<<"A=M"<<'\n'
					<<"D=M"<<'\n'*/
			outFile	<<"@"<<fnprefix<<index<<'\n'
					<<"M=D"<<'\n';
			break;
		case 7:
			outFile  << "//pop pointer "<<index<<'\n';
			SP2D();
				/*<< "@SP" << '\n' //rep2
				<<"M=M-1"<<'\n'
				<<"A=M"<<'\n'
				<<"D=M"<<'\n'*/ 
			outFile <<"@"<<3+index<<'\n'
				<<"M=D"<<'\n';
			break;
		case 8:
			outFile  << "//pop temp "<<index<<'\n';
			SP2D();
					/*<< "@SP" << '\n' //rep2
					<<"M=M-1"<<'\n'
					<<"A=M"<<'\n'
					<<"D=M"<<'\n'*/
			outFile	<<"@"<<5+index<<'\n'
					<<"M=D"<<'\n';
			break;
		
		default:
			outFile<<"Bad translation in PUSH or POP!"<<'\n';
		}
	}
}

void CodeWriter::SingleVm2Asm() {
    	
	int C_Type = parser.commandType();
	auto arg1 = parser.arg1();
	int arg2 = parser.arg2();
	if (C_Type == C_PUSH || C_Type == C_POP){
		writePushPop(C_Type,arg1,arg2);
	}
	
	if (C_Type == C_ARITHMETIC){
		writeArithmetic(arg1);
	}
}

void CodeWriter::Vm2Asm(){
		
	while (parser.getcurChar() != EOF){
		SingleVm2Asm();
		parser.advance();
	}
	
}
