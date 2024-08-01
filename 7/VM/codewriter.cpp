#include "codewriter.h"
#include <map>

CodeWriter::CodeWriter(const std::string& filename)
	: filename(filename), parser(Parser(filename))
{
	file.open(filename);
	if (!file.is_open()) {
		std::cerr << "Error opening file" << '\n';
		exit(1);
	}
}



CodeWriter::~CodeWriter() {
	if (file.is_open()) {
		file.close();
	}
}


std::map<std::string,char> AL2OPMap = { {"add",'+'}, {"sub",'-'}, {"and",'&'}, {"or",'|'} };

std::map<std::string,int> Cmd2Int = {{"add",1}, {"sub",2}, {"neg",3}, {"eq",4}, {"gt",5}, {"lt",6}, {"and",7}, {"or",8}, {"not",9}};

std::map<std::string,char> AL1OPMap = { {"neg",'-'}, {"not",'!'}};

std::map<std::string,std::string> CompMap = { {"eq","EQ"}, {"gt","GT"}, {"lt","LT"}};



void CodeWriter::writeArithmetic(const std::string& command){
	switch (Cmd2Int[command]){
	case 1: case 2: case 7: case 8:
		std::cout<< "@SP" << '\n' 
				<<"A=M"<<'\n'
				<<"D=M"<<'\n'
				<< "@SP" << '\n' 
				<<"M=M-1"<<'\n' 
				<<"A=M"<<'\n'
				<<"D=M"<<AL2OPMap[command]<<"D"<<'\n'
				<< "@SP" << '\n' 
				<<"M=M-1"<<'\n'
				<<"A=M"<<'\n'
				<<"M=D"<<'\n'
				<< "@SP" << '\n' 
				<<"M=M+1"<<'\n';
				
		break;
	
	case 3: case 9:
		std::cout<< "@SP" << '\n' 
				<<"A=M"<<'\n'
				<<"D="<<AL1OPMap[command]<<"M"<<'\n'
				<< "@SP" << '\n' 
				<<"M=M-1"<<'\n' 
				<<"A=M"<<'\n'
				<<"M=D"<<'\n'
				<< "@SP" << '\n' 
				<<"M=M+1"<<'\n';
		break;
	
	case 4: case 5: case 6:
		std::cout<< "@SP" << '\n' 
				<<"A=M"<<'\n'
				<<"D=M"<<'\n'
				<< "@SP" << '\n' 
				<<"M=M-1"<<'\n' 
				<<"A=M"<<'\n'
				<<"D=M-D"<<'\n'
				<<"@"<<CompMap[command]<<'\n'
				<<"D;J"<<CompMap[command]<<'\n'
				<<"D=0"<<'\n'
				<<"("<<CompMap[command]<<")"<<'\n'
				<<"D=-1"<<'\n'
				<< "@SP" << '\n'
				<<"M=M-1"<<'\n'
				<<"A=M"<<'\n'
				<<"M=D"<<'\n'
				<< "@SP" << '\n'
				<<"M=M+1"<<'\n';
		break;
	default:
		std::cout<<"Bad translation in AL!"<<'\n';
	
	}
}


std::map<std::string,std::string> SegMap = {{"local","LCL"}, {"argument","ARG"}, {"this","THIS"}, {"that","THAT"}};

std::map<std::string,int> Seg2Int = {{"argument",1},{"local",2},{"static",3},{"constant",4}, {"this",5}, {"that",6},{"pointer",7},{"temp",8} };

void CodeWriter::writePushPop(int ct, const std::string& segment, int index){
	if (ct == C_PUSH){
		switch (Seg2Int[segment]){
		case 4:
			std::cout << "//push constant "<<index<<'\n'
					<<"@"<<index<<'\n'
					<<"D=A"<<'\n'
					<< "@SP" << '\n'
					<<"A=M"<<'\n'
					<<"M=D"<<'\n'
					<< "@SP" << '\n' 
					<<"M=M+1"<<'\n';
			break;
		case 2: case 1: case 5: case 6:
			std::cout << "//push "<<segment<<" "<<index<<'\n'
					<<"@"<<index<<'\n'
					<<"D=A"<<'\n'
					<<"@"<<SegMap[segment]<<'\n'
					<<"A=M+D"<<'\n'
					<<"D=M"<<'\n'
					<< "@SP" << '\n' 
					<<"A=M"<<'\n'
					<<"M=D"<<'\n'
					<<"@SP"<< '\n' 
					<<"M=M+1"<<'\n';
			break;
		case 7:
			if (index==0)
				std::cout << "//push pointer "<<index<<'\n'
					<<"@THIS"<<'\n'
					<< "A=M"<<'\n'
					<<"D=M"<<'\n'
					<< "@SP" << '\n' 
					<<"A=M"<<'\n'
					<<"M=D"<<'\n'
					<< "@SP" << '\n' 
					<<"M=M+1"<<'\n';
			else if (index==1)
				std::cout <<"@THAT"<<'\n'
					<< "A=M"<<'\n'
					<<"D=M"<<'\n'
					<< "@SP" << '\n' 
					<<"A=M"<<'\n'
					<<"M=D"<<'\n'
					<< "@SP" << '\n' 
					<<"M=M+1"<<'\n';
			break;
		case 8:
			std::cout << "//push temp "<<index<<'\n'
					<<"@R"<<5 + index<<'\n'
					<< "A=M"<<'\n'
					<<"D=M"<<'\n' 
					<< "@SP" << '\n' 
					<<"A=M"<<'\n'
					<<"M=D"<<'\n'
					<< "@SP" << '\n' 
					<<"M=M+1"<<'\n';
			break;
		case 3:  //wrong!!!
			std::cout << "//push static "<<index<<'\n'
					<<"@"<<16 + index<<'\n'
					<< "A=M"<<'\n'
					<<"D=M"<<'\n' 
					<< "@SP" << '\n' 
					<<"A=M"<<'\n'
					<<"M=D"<<'\n'
					<< "@SP" << '\n' 
					<<"M=M+1"<<'\n';
			break;
		default:
			std::cout<<"Bad translation!"<<'\n';
		}
	}	
	if (ct == C_POP){
		switch (Seg2Int[segment]){
		case 2: case 1: case 5: case 6:
			std::cout  << "//pop "<<SegMap[segment]<<" "<<index<<'\n'
					<<"@SP"<<'\n'
					<<"A=M"<<'\n'
					<<"D=M"<<'\n'
					<<"@SP"<< '\n' 
					<<"M=M-1"<<'\n'
					<<"@"<<index<<'\n'
					<<"D=A"<<'\n'
					<<"@"<<SegMap[segment]<<'\n'
					<<"A=M+D"<<'\n' 
					<<"M=D"<<'\n';
			break;
		case 7:
			if (index==0)
				std::cout << "//pop pointer "<<index<<'\n'
					<<"@SP"<<'\n'
					<<"A=M"<<'\n'
					<<"D=M"<<'\n'
					<<"@SP"<< '\n' 
					<<"M=M-1"<<'\n'
					<<"@THIS"<<'\n'
					<<"A=M"<<'\n'
					<<"M=D"<<'\n';
			else if (index==1)
				std::cout  <<"@SP"<<'\n'
					<<"A=M"<<'\n'
					<<"D=M"<<'\n'
					<<"@SP"<< '\n' 
					<<"M=M-1"<<'\n'
					<<"@THAT"<<'\n'
					<<"A=M"<<'\n'
					<<"M=D"<<'\n';
			break;
		case 8:
			std::cout  << "//pop temp "<<index<<'\n'
					<<"@SP"<<'\n'
					<<"A=M"<<'\n'
					<<"D=M"<<'\n'
					<<"@SP"<< '\n' 
					<<"M=M-1"<<'\n'
					<<"@R"<<5+index<<'\n'
					<<"A=M"<<'\n'
					<<"M=D"<<'\n';
			break;
		case 3:  //wrong!!!
			std::cout  << "//pop static "<<index<<'\n'
					<<"@SP"<<'\n'
					<<"A=M"<<'\n'
					<<"D=M"<<'\n'
					<<"@SP"<< '\n' 
					<<"M=M-1"<<'\n'
					<<"@R"<<16 + index<<'\n'//???
					<<"A=M"<<'\n'
					<<"M=D"<<'\n';
			break;
		default:
			std::cout<<"Bad translation in PUSH or POP!"<<'\n';
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
