#include "VMWriter.h"

std::map<Segment,std::string> Seg2Str ={
							{Segment::CONSTANT,"constant"},{Segment::ARGUMENT,"argument"},{Segment::LOCAL,"local"},{Segment::STATIC,"static"},
							{Segment::THIS,"this"},{Segment::THAT,"that"},{Segment::POINTER,"pointer"},{Segment::TEMP,"temp"}
							};
							
std::map<Command,std::string> Cmd2Str ={
							{Command::ADD,"add"},{Command::SUB,"sub"},{Command::NEG,"neg"},{Command::EQ,"eq"},
							{Command::GT,"gt"},{Command::LT,"lt"},{Command::AND,"and"},{Command::OR,"or"},{Command::NOT,"not"}
							};

VMWriter::VMWriter(const std::string& filename){
	size_t commapos = filename.find('.');
	std::string outfn;
	if (commapos != std::string::npos)
		outfn = filename.substr(0,commapos) + "Test.vm";
	outfs.open(outfn);
	if (!outfs.is_open()) {
		std::cerr << "Error opening file" << '\n';
		exit(1);
	}
}

VMWriter::~VMWriter(){
	if (outfs.is_open()){
		outfs.close();
	}
}


void VMWriter::writePush(Segment seg,int index){
	outfs << "push " << Seg2Str[seg] <<" "<< index <<'\n';
}


void VMWriter::writePop(Segment seg,int index){
	outfs << "pop " << Seg2Str[seg] <<" "<< index <<'\n';
}

void VMWriter::writeArithmetic(Command cmd){
	outfs << Cmd2Str[cmd] <<'\n';
}


void VMWriter::writeLabel(const std::string& label){
	outfs << "label "<< label <<'\n';
}


void VMWriter::writeGoto(const std::string& label){
	outfs << "goto " << label << '\n';
}


void VMWriter::writeIfGoto(const std::string& label){
	outfs << "if-goto " << label << '\n';
}


void VMWriter::writeCall(const std::string& name,int nArgs){
	outfs << "call " << name << " " << nArgs << '\n';
}


void VMWriter::writeFunction(const std::string& name,int nVars){
	outfs << "function " << name << " " << nVars << '\n';
}
void VMWriter::writeReturn(){
	outfs << "return" << '\n';
}
