#include "codewriter.h"
#include <map>

CodeWriter::CodeWriter(const std::string& filename)
	: filename(filename), parser(Parser(filename)), curFunName(" "),callIndex(-1)
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
	
	case 4: case 5: case 6: //!!!
		outFile << "//"<<command<<'\n';
		SP2D();
		
		outFile << "@SP" << '\n' 
				<<"M=M-1"<<'\n' 
				<<"A=M"<<'\n'
				<<"D=M-D"<<'\n'
				<<"@"<<CompMap[command]<<'\n'
				<<"D;J"<<CompMap[command]<<'\n'
				<<"D=0"<<'\n'
				<<"("<<CompMap[command]<<")"<<'\n'
				<<"D=-1"<<'\n';
		D2SP();
				/*<< "@SP" << '\n' //rep1 
				<<"A=M"<<'\n'
				<<"M=D"<<'\n'
				<< "@SP" << '\n' 
				<<"M=M+1"<<'\n';*/
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
			if (index==0) {
				outFile << "//push pointer "<<index<<'\n'
					<<"@THIS"<<'\n'
					<<"D=M"<<'\n';
				D2SP();
				
			}
			else if (index==1){
				outFile << "//push pointer "<<index<<'\n'
					<<"@THAT"<<'\n'
					<<"D=M"<<'\n';
				D2SP();
			}
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
			if (index==0){
				outFile << "//pop pointer "<<index<<'\n';
				SP2D();
				
				outFile <<"@THIS"<<'\n'
					  <<"M=D"<<'\n';
			}
			else if (index==1){
				outFile  << "//pop pointer "<<index<<'\n';
				SP2D();
				 
				outFile <<"@THAT"<<'\n'
					 <<"M=D"<<'\n';
			}
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
			<<"0,JMP"<<'\n';
}
	
void CodeWriter::writeIf(const std::string& label){

	outFile << "//if-goto "<<curFunName+"$"+label<<'\n';
	SP2D();
	
	outFile	<<"@"<<curFunName+"$"+label<<'\n'
			<<"D,JNE"<<'\n';
}

void CodeWriter::writeFunction(const std::string& funName,int nVars){  //Check!!!!!!!!!!!!!!!!!!!!!!!
	outFile << "//function "<<funName<<" "<<nVars<<'\n';
	
	curFunName = funName;
	writeLabel(funName);
	for (int i=0;i<nVars;i++){
		writePushPop(C_PUSH, "constant", 0);
		writePushPop(C_POP, "local", i);
	}
}

	
void CodeWriter::writeCall(const std::string& funName,int nArgs){
	outFile << "//call "<<funName<<" "<<nArgs<<'\n';

	//push returnAddress
	callIndex++;
	outFile	<< "@" <<funName<<"$ret."<<callIndex<<'\n'
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
	writeGoto(funName);
	
	//(returnAddress)
	writeLabel(funName+"$ret."+std::to_string(callIndex));
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
			<< "D=A+1"<<'\n'
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
			<< "AM=M-1"<<'\n'
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
		writePushPop(C_Type,arg1,arg2);
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
		
	while (parser.getcurChar() != EOF){
		SingleVm2Asm();
		parser.advance();
	}
	
}
