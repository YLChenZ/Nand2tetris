#include "codewriter.h"
#include <map>



CodeWriter::CodeWriter(const std::string& filename, std::ofstream& outFile)
    : filename(filename), parser(Parser(filename)), curFunName(" "), callIndex(-1), compIndex(-1), outFile(outFile) {
    // File open check
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << '\n';
        exit(1);
    }
}

CodeWriter::~CodeWriter() {}


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

		outFile <<"@SP"<<'\n' 
				<<"M=M-1"<<'\n' 
				<<"A=M"<<'\n'
				<<"D=M-D"<<'\n';
		D2SP();	
			
		break;
	case 3: case 9:
		outFile << "//"<<command<<'\n'
				<< "@SP" << '\n'
				<<"M=M-1"<<'\n'
				<<"A=M"<<'\n'
				<<"D="<<AL1OPMap[command]<<"M"<<'\n';
		D2SP();
		
		break;
	
	case 4: case 5: case 6: 
		compIndex++;
		outFile << "//"<<command<<'\n';
		SP2D();
		
		outFile << "@SP" << '\n' 
				<<"M=M-1"<<'\n' 
				<<"A=M"<<'\n'
				<<"D=M-D"<<'\n'
				<<"@"<<CompMap[command]<<"_"<<compIndex<<'\n'
				<<"D;J"<<CompMap[command]<<'\n'
				<<"@SP"<<'\n'
				<< "A=M"<<'\n'
				<< "M=0"<<'\n'
				<<"@"<<CompMap[command]<<"_CONTINUE"<<"_"<<compIndex<<'\n'
				<<"0;JMP"<<'\n'
				<<"("<<CompMap[command]<<"_"<<compIndex<<")"<<'\n'
				<<"@SP"<<'\n'
				<< "A=M"<<'\n'
				<< "M=-1"<<'\n'
				<<"("<<CompMap[command]<<"_CONTINUE"<<"_"<<compIndex<<")"<<'\n'
				<<"@SP"<<'\n'
				<< "M=M+1"<<'\n';
		
		break;
	default:
		outFile<<"Bad translation in AL!"<<'\n';
	
	}
}


std::map<std::string,std::string> SegMap = {{"local","LCL"}, {"argument","ARG"}, {"this","THIS"}, {"that","THAT"}};

std::map<std::string,int> Seg2Int = {{"argument",1},{"local",2},{"static",3},{"constant",4}, {"this",5}, {"that",6},{"pointer",7},{"temp",8} };


void CodeWriter::writePushPop(int ct, const std::string& segment, int index){
	size_t lastSlash = filename.find_last_of("/");
	
	auto fullname = filename.substr(lastSlash+1);
	
	auto fnprefix = fullname.substr(0,fullname.size()-2);
		
	//std::cout<<filename<<'\n';
		
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
			
			break;
		case 3:
			outFile << "//push static "<<index<<'\n'
					<<"@"<<fnprefix<<index<<'\n'
					<<"D=M"<<'\n';
			D2SP();
			
			break;
		case 4:
			outFile << "//push constant "<<index<<'\n'
					<<"@"<<index<<'\n'
					<<"D=A"<<'\n';
			D2SP();
			
			break;
		case 7:
			outFile << "//push pointer "<<index<<'\n'
				<<"@"<<3 + index<<'\n'
				<<"D=M"<<'\n';
			D2SP();
			
			break;
		case 8:
			outFile << "//push temp "<<index<<'\n'
					<<"@"<<5 + index<<'\n'
					<<"D=M"<<'\n';
			D2SP();
			
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
			
			outFile	<<"@R13"<<'\n'
					<<"A=M"<<'\n'
					<<"M=D"<<'\n';			
			break;
		case 3:
			outFile  << "//pop static "<<index<<'\n';
			SP2D();
			
			outFile	<<"@"<<fnprefix<<index<<'\n'
					<<"M=D"<<'\n';
			break;
		case 7:
			outFile  << "//pop pointer "<<index<<'\n';
			SP2D();

			outFile <<"@"<<3+index<<'\n'
				<<"M=D"<<'\n';
			break;
		case 8:
			outFile  << "//pop temp "<<index<<'\n';
			SP2D();
			
			outFile	<<"@"<<5+index<<'\n'
					<<"M=D"<<'\n';
			break;
		
		default:
			outFile<<"Bad translation in PUSH or POP!"<<'\n';
		}
	}
}


void CodeWriter::writeLabel(const std::string& label){
	outFile	<< "//label "<<curFunName+"$"+label<<'\n'
			<<"("<<curFunName+"$"+label<<")"<<'\n';
}
	
void CodeWriter::writeGoto(const std::string& label){
	outFile	<<"//goto "<<curFunName+"$"+label<<'\n'
			<<"@"<<curFunName+"$"+label<<'\n'
			<<"0;JMP"<<'\n';
}
	
void CodeWriter::writeIf(const std::string& label){

	outFile << "//if-goto "<<curFunName+"$"+label<<'\n';
	SP2D();
	
	outFile	<<"@"<<curFunName+"$"+label<<'\n'
			<<"D;JNE"<<'\n';
}

void CodeWriter::writeFunction(const std::string& funName,int nVars){ 
	outFile << "//function "<<funName<<" "<<nVars<<'\n';
	
	curFunName = funName;
	
	outFile	<<"("<<funName<<")"<<'\n';
			
	for (int i=0;i<nVars;i++){
		writePushPop(C_PUSH, "constant", 0);
		writePushPop(C_POP, "local", i);
	}
	
	outFile	<<"@"<<nVars<<'\n'
			<<"D=A"<<'\n'
			<<"@SP"<<'\n'
			<<"M=D+M"<<'\n';
}

	
void CodeWriter::writeCall(const std::string& funName,int nArgs){
	outFile << "//call "<<funName<<" "<<nArgs<<'\n';

	//push returnAddress
	callIndex++;
	outFile	<< "@" <<curFunName<<"_"<<funName<<"$ret."<<callIndex<<'\n'
			<< "D=A"<<'\n';
	D2SP();
	

	//push LCL,ARG,THIS,THAT
	std::vector<std::string> seg = {"LCL","ARG","THIS","THAT"};
	for (auto& s : seg){
		outFile	<< "@" <<s<<'\n'
				<< "D=M"<<'\n';
		D2SP();
	}
	
	//ARG = SP-5-nArgs
	outFile	<< "@SP" <<'\n'
			<< "D=M"<<'\n'
			<< "@" <<5+nArgs<<'\n'
			<< "D=D-A"<<'\n'
			<< "@ARG" <<'\n'
			<< "M=D"<<'\n';
			
	//LCL=SP
	outFile	<< "@SP" <<'\n'
			<< "D=M"<<'\n'
			<< "@LCL" <<'\n'
			<< "M=D"<<'\n';
	
	//goto f
	outFile	<<"//goto "<<funName<<'\n'
			<<"@"<<funName<<'\n'
			<<"0;JMP"<<'\n';
	
	//(returnAddress)
	outFile	<< "//label "<<curFunName<<"_"<<funName<<"$ret."<<callIndex<<'\n'
			<<"("<<curFunName<<"_"<<funName<<"$ret."<<callIndex<<")"<<'\n';
}


	
void CodeWriter::writeReturn(){
	outFile	<< "//return from " <<curFunName<<'\n';

	//frame = LCL
	outFile	<< "@LCL" <<'\n'
			<< "D=M"<<'\n'
			<< "@R14" <<'\n'
			<< "M=D"<<'\n';
	
	//retAddr = *(frame-5)
	outFile	<< "@5" <<'\n'
			<< "A=D-A"<<'\n'
			<< "D=M"<<'\n'
			<< "@R15" <<'\n'
			<< "M=D" <<'\n';
			
	//*ARG = POP()
	SP2D();
	outFile	<< "@ARG" <<'\n'
			<< "A=M" <<'\n'
			<< "M=D" <<'\n';
	
	//SP = ARG+1
	outFile	<< "@ARG" <<'\n'
			<< "D=M+1"<<'\n'
			<<"@SP"<<'\n'
			<<"M=D"<<'\n';
			
	//THAT = *(frame-1)
	outFile	<< "@R14" <<'\n'
			<< "AM=M-1"<<'\n'
			<< "D=M" <<'\n' 
			<< "@THAT" <<'\n'
			<< "M=D"<<'\n';
	
	//THIS = *(frame-2)
	outFile	<< "@R14" <<'\n'
			<< "AM=M-1"<<'\n'
			<< "D=M" <<'\n'
			<< "@THIS" <<'\n'
			<< "M=D"<<'\n';
			
	//ARG = *(frame-3)
	outFile	<< "@R14" <<'\n'
			<< "AM=M-1"<<'\n'
			<< "D=M" <<'\n'
			<< "@ARG" <<'\n'
			<< "M=D"<<'\n';
			
	//LCL = *(frame-4)
	outFile	<< "@R14" <<'\n'
			<< "A=M-1"<<'\n'
			<< "D=M" <<'\n'
			<< "@LCL" <<'\n'
			<< "M=D"<<'\n';
	
	//goto retAddr
	outFile	<< "@R15" <<'\n'
			<< "A=M"<<'\n'
			<< "0;JMP"<<'\n';
	
}







void CodeWriter::SingleVm2Asm() {
    	
	int C_Type = parser.commandType();
	auto arg1 = parser.arg1();
	int arg2 = parser.arg2();
	
	switch (C_Type){
	case -1:
		writeArithmetic(arg1);
		break;
	case -2: case -3:
		writePushPop(C_Type,arg1,arg2);
		break;	
	case -4:
		writeLabel(arg1);
		break;
	case -5:
		writeGoto(arg1);
		break;
	case -6:
		writeIf(arg1);
		break;
	case -7:
		writeFunction(arg1,arg2);
		break;
	case -8:
		writeReturn();
		break;
	case -9:
		writeCall(arg1,arg2);
		break;
	default:
		std::cout << "Error in SingleVm2Asm." <<'\n';	
	}

}



void CodeWriter::Vm2Asm(){
		
	while (parser.getcurChar() != EOF && parser.getcurCmd()!="EOF") {
		SingleVm2Asm();
		parser.advance();
	}
	
}
