#include "CompilationEngine.h"


CompilationEngine::CompilationEngine(const std::string& filename)
	: tok(Tokenizer(filename)), vmw(VMWriter(filename)), ClassLevelSt(SymTab()), SubroutineLevelSt(SymTab()),
	 classname(" "), funname(" "), FuncKind(UNKNOWN), IfLabelIndex(0),WhileLabelIndex(0)
{
	/*
	size_t commapos= filename.find('.');
	std::string outfn;
	if (commapos != std::string::npos){
		outfn = filename.substr(0,commapos) + "Ast.xml";
	}
	
	out.open(outfn);
	
	if (!out.is_open()) {
		std::cerr << "Error opening file" << '\n';
		exit(1);
	}
	*/
}
/*
CompilationEngine::~CompilationEngine(){
	if (out.is_open()){
		out.close();
	}
}
*/

bool CompilationEngine::isDataType(){
	return tok.keyWord() == INT || tok.keyWord() == BOOLEAN || tok.keyWord() == CHAR;
}

bool CompilationEngine::isRetType(){
	return tok.keyWord() == INT || tok.keyWord() == BOOLEAN || tok.keyWord() == CHAR || tok.keyWord() == VOID;
}

bool CompilationEngine::isOp(){
	return tok.symbol() == '+' || tok.symbol() == '-' || tok.symbol() == '*' || 
			tok.symbol() == '/' || tok.symbol() == '&' || tok.symbol() == '|' ||
			tok.symbol() == '<' || tok.symbol() == '>' || tok.symbol() == '=';
		
}

bool CompilationEngine::isunaryOp(){
	return tok.symbol() == '-' || tok.symbol() == '~';	
}
	
void CompilationEngine::compileClass(){
	if (tok.keyWord() == CLASS){
		tok.advance();
	}
	else{
		return;
	}
	
	if (tok.tokenType() == TokType::IDENTIFIER){
		classname = tok.getCurToken().TokNm;
		tok.advance();
	}
	else {
		std::cout << "Excepted an identifier in compileClass"<<'\n';
		return;
	}
	if (tok.symbol() == '{'){
		tok.advance();
	}
	else{
		std::cout << "Excepted '{'"<<'\n';
		return;
	}
	while (tok.keyWord() == FIELD || tok.keyWord() == STATIC)
		compileClassVarDec();
	while (tok.keyWord() == CONSTRUCTOR || tok.keyWord() == FUNCTION || tok.keyWord() == METHOD)
		compileSubroutine();
	if (tok.symbol() == '}'){
		tok.advance();
	}
	else{
		std::cout << "Excepted '}' in compileClass"<<'\n';
		return;
	}
}


void CompilationEngine::compileClassVarDec(){
	std::string name,type;
	Kind kind;
	
	if (tok.keyWord() == FIELD || tok.keyWord() == STATIC){
		if (tok.keyWord() == FIELD)
			kind = Kind::FIELD;
		if (tok.keyWord() == STATIC)
			kind = Kind::STATIC;
		tok.advance();
	}
	else {
		std::cout << "Excepted 'filed' or 'static' in compileClassVarDec"<<'\n';
		return;
	}
	
	if (isDataType() || tok.tokenType() == TokType::IDENTIFIER){
		type = tok.getCurToken().TokNm;
		tok.advance();
	}
	else {
		std::cout << "Excepted a type like 'int' or classname in compileClassVarDec"<<'\n';
		return;
	}
	if (tok.tokenType() == TokType::IDENTIFIER){
		name = tok.getCurToken().TokNm;
		tok.advance();
	}
	else {
		std::cout << "Excepted an identifier in compileClassVarDec"<<'\n';
		return;
	}
	
	ClassLevelSt.add(name,type,kind);
	
	while (true){
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ','){
			tok.advance();
			if (tok.tokenType() == TokType::IDENTIFIER){
				name = tok.getCurToken().TokNm;
				ClassLevelSt.add(name,type,kind);
				tok.advance();
			}
			else {
				std::cout << "Excepted an identifier in compileClassVarDec While"<<'\n';
				return;
			}
		}
		else break;
	}
	
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ';'){
		tok.advance();
	}
	else {
		std::cout << "Excepted ';' in compileClassVarDec end"<<'\n';
		return;
	}
}


void CompilationEngine::compileSubroutine(){
	SubroutineLevelSt.reset(); //reset SubroutineLevelSt;
	IfLabelIndex = 0;
	WhileLabelIndex = 0;
	FuncKind = tok.keyWord();
	if (FuncKind == CONSTRUCTOR || FuncKind == FUNCTION || FuncKind == METHOD){
		tok.advance();
	}
	else {
		std::cout << "Excepted 'constructor', 'function' or 'method' in compileSubroutine"<<'\n';
		return;
	}
	if (isRetType() || tok.tokenType() == TokType::IDENTIFIER){
		tok.advance();
	}
	else {
		std::cout << "Excepted 'int', 'boolean', 'char', 'void' or a class name in compileSubroutine retType"<<'\n';
		return;
	}
	
	if (tok.tokenType() == TokType::IDENTIFIER){
		funname = classname + '.' + tok.identifier();
		tok.advance();
	}
	else {
		std::cout << "Excepted an identifier in compileSubroutine name"<<'\n';
		return;
	}
	
	compileParameterList();
	
	compileSubroutineBody();
	auto Map = SubroutineLevelSt.getSTMap();
	for (auto e : Map){
		std::cout << e.first <<" "<< e.second.type << " " << e.second.index<<'\n';
	}
}


void CompilationEngine::compileParameterList(){
	std::string name,type;
	Kind kind = Kind::ARG;
	
	if (FuncKind == METHOD){
		SubroutineLevelSt.add("this",classname,kind);
	}
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '('){
		tok.advance();
	}
	else {
		std::cout << "Excepted '(' in compileParameterList"<<'\n';
		return;
	}
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ')'){
		tok.advance();
		return;
	}
	else if (isDataType() || tok.tokenType() == TokType::IDENTIFIER){
		type = tok.getCurToken().TokNm;
		tok.advance();
	}
	else {
		std::cout << "Excepted 'int', 'bool', 'char' or a class name in compileParameterList"<<'\n';
		return;
	}
	if (tok.tokenType() == TokType::IDENTIFIER){
		name = tok.identifier();
		SubroutineLevelSt.add(name,type,kind);
		tok.advance();
	}
	else {
		std::cout << "Excepted an identifier in compileParameterList"<<'\n';
		return;
	}
	
	while (true){
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ','){
			tok.advance();
			if (isDataType() || tok.tokenType() == TokType::IDENTIFIER){
				type = tok.getCurToken().TokNm;
				tok.advance();
			}
			else {
				std::cout << "Excepted 'int', 'bool', 'char' or a class name in compileParameterList While"<<'\n';
				return;
			}
			if (tok.tokenType() == TokType::IDENTIFIER){
				name = tok.identifier();
				SubroutineLevelSt.add(name,type,kind);
				tok.advance();
			}
			else {
				std::cout << "Excepted an identifier in compileParameterList While"<<'\n';
				return;
			}
		}
		else break;
	}
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ')'){
		tok.advance();
	}
	else {
		std::cout << "Excepted ')' in in compileParameterList"<<'\n';
		return;
	}
}


void CompilationEngine::compileSubroutineBody(){
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '{'){
		tok.advance();
	}
	else {
		std::cout << "Excepted '{' in compileSubroutineBody"<<'\n';
		return;
	}
	
	while (tok.keyWord() == VAR)
		compileVarDec();

	int nVars = SubroutineLevelSt.varCount(Kind::VAR);
	int nFlds = ClassLevelSt.varCount(Kind::FIELD);
	
	if (FuncKind == CONSTRUCTOR){
		vmw.writeFunction(funname,nVars);
		vmw.writePush(Segment::CONSTANT,nFlds);
		vmw.writeCall("Memory.alloc",1);
		vmw.writePop(Segment::POINTER,0);
	}
	else if (FuncKind == METHOD){
		vmw.writeFunction(funname,nVars);
		vmw.writePush(Segment::ARGUMENT,0);
		vmw.writePop(Segment::POINTER,0);
	}
	else if (FuncKind == FUNCTION){
		vmw.writeFunction(funname,nVars);
	}
	
	compileStatements();
	
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '}'){
		tok.advance();
	}
	else {
		std::cout << "Excepted '}' in compileSubroutineBody"<<'\n';
		return;
	}
}


void CompilationEngine::compileVarDec(){
	std::string name,type;
	Kind kind;
	
	if (tok.keyWord() == VAR){
		kind = Kind::VAR;
		tok.advance();
	}
	if (isDataType() || tok.tokenType() == TokType::IDENTIFIER){
		type = tok.getCurToken().TokNm;
		tok.advance();
	}
	else {
		std::cout << "Excepted 'int', 'bool', 'char' or a class name"<<'\n';
		return;
	}
	if (tok.tokenType() == TokType::IDENTIFIER){
		name = tok.getCurToken().TokNm;
		tok.advance();
	}
	else {
		std::cout << "Excepted an identifier in compileVarDec"<<'\n';
		return;
	}
	SubroutineLevelSt.add(name,type,kind);
	
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ';'){
		tok.advance();
		return;
	}
	while (true){
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ','){
			tok.advance();
			if (tok.tokenType() == TokType::IDENTIFIER){
				name = tok.getCurToken().TokNm;
				tok.advance();
				SubroutineLevelSt.add(name,type,kind);
			}
			else {
				std::cout << "Excepted an identifier in compileVarDec While"<<'\n';
				return;
			}
		}
		else break;
	}
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ';'){
		tok.advance();
	}
	else {
		std::cout << "Excepted ';' in compileVarDec"<<'\n';
		return;
	}
}


void CompilationEngine::compileStatements(){
	while (true){
		if (tok.keyWord() == LET)
			compileLet();
		else if (tok.keyWord() == IF)
			compileIf();
		else if (tok.keyWord() == WHILE)
			compileWhile();
		else if (tok.keyWord() == DO)
			compileDo();
		else if (tok.keyWord() == RETURN)
			compileReturn();
		else break;
	}
}

Value CompilationEngine::findInST(){
	Value v = {" ",Kind::NONE,-1};
	auto tokName = tok.getCurToken().TokNm;
	auto clmap = ClassLevelSt.getSTMap();
	auto slmap = SubroutineLevelSt.getSTMap();
	
	auto clfind = clmap.find(tokName);
	auto slfind = slmap.find(tokName);
	
	if (slfind != slmap.end()){
		v = slmap[tokName];
	}
	else if (clfind != clmap.end()){
		v = clmap[tokName];
	}
	else {
		//std::cout << "undefined "<< tokName <<'\n';
		return v;
	}
}

std::map<Kind,Segment> Kind2Seg = 
{
{Kind::STATIC,Segment::STATIC},{Kind::FIELD,Segment::THIS},
{Kind::ARG,Segment::ARGUMENT},{Kind::VAR,Segment::LOCAL}
};

void CompilationEngine::compileLet(){
	std::string LhsName, RhsName, LhsType, RhsType;
	Kind LhsKind, RhsKind;
	int index;
	Value v;

	if (tok.keyWord() == LET){
		tok.advance();
	}
	if (tok.tokenType() == TokType::IDENTIFIER){
		LhsName = tok.getCurToken().TokNm;
		v = findInST();
		if (v.index == -1){
			std::cout << LhsName << " undefined!"<<'\n';
			return;
		}	
		tok.advance();
	}
	else {
		std::cout << "Excepted an identifier in compileLet"<<'\n';
		return;
	}
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '['){//arr[i]
		tok.advance();
		//push i
		compileExpression();
		//push arr
		vmw.writePush(Kind2Seg[v.kind],v.index);
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ']'){
			vmw.writeArithmetic(Command::ADD);  //add
			//vmw.writePop(Segment::POINTER,1);  //pop pointer 1 (that)
			tok.advance();
		}
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '='){
			tok.advance();
		}
		else {
			std::cout << "Excepted '=' in compileLet"<<'\n';
			return;
		}
		compileExpression();
		//pop temp 0
		vmw.writePop(Segment::TEMP,0);
		//pop pointer 1
		vmw.writePop(Segment::POINTER,1);
		//push temp 0
		vmw.writePush(Segment::TEMP,0);
		//pop that 0
		vmw.writePop(Segment::THAT,0);
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ';'){
			tok.advance();
		}
		else {
			std::cout << "Excepted ';' in compileLet"<<'\n';
			return;
		}
		return;
	}
	
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '='){
		tok.advance();
	}
	else {
		std::cout << "Excepted '=' in compileLet"<<'\n';
		return;
	}
	compileExpression();
	//pop varName
	vmw.writePop(Kind2Seg[v.kind],v.index);
	
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ';'){
		tok.advance();
	}
	else {
		std::cout << "Excepted ';' in compileLet"<<'\n';
		return;
	}
}


void CompilationEngine::compileIf(){
	std::string label1 = "IF_TRUE" +std::to_string(IfLabelIndex);
	std::string label2 = "IF_FALSE" +std::to_string(IfLabelIndex);
	std::string label3 = "IF_END" +std::to_string(IfLabelIndex);
	IfLabelIndex++;
	if (tok.keyWord() == IF){
		tok.advance();
	}
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '('){
		tok.advance();
	}
	else {
		std::cout << "Excepted '('"<<'\n';
		return;
	}
	compileExpression();
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ')'){
		tok.advance();
	}
	else {
		std::cout << "Excepted ')'"<<'\n';
		return;
	}
	
	//vmw.writeArithmetic(Command::NOT); //not
	
	//if-goto label1
	vmw.writeIfGoto(label1);
	
	//goto label2
	vmw.writeGoto(label2);
	
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '{'){
		tok.advance();
	}
	else {
		std::cout << "Excepted '{'"<<'\n';
		return;
	}
	
	//label1
	vmw.writeLabel(label1);
	compileStatements();
	
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '}'){
		tok.advance();
	}
	else {
		std::cout << "Excepted '}'"<<'\n';
		return;
	}
	if (tok.keyWord() == ELSE){
		//goto label3
		vmw.writeGoto(label3);
		tok.advance();
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '{'){
			tok.advance();
		}
		else {
			std::cout << "Excepted '{'"<<'\n';
			return;
		}
		//label2
		vmw.writeLabel(label2);
		compileStatements();
		
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '}'){
			//goto label3
			//vmw.writeGoto(label3);
			tok.advance();
		}
		else {
			std::cout << "Excepted '}'"<<'\n';
			return;
		}
		//label3
		vmw.writeLabel(label3);
		return;
	}
	//label2
	vmw.writeLabel(label2);
	return;
}


void CompilationEngine::compileWhile(){
	std::string label1 = "WHILE_EXP" + std::to_string(WhileLabelIndex);
	std::string label2 = "WHILE_END" + std::to_string(WhileLabelIndex);
	WhileLabelIndex++;
	if (tok.keyWord() == WHILE){
		tok.advance();
	}
	//label1
	vmw.writeLabel(label1);
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '('){
		tok.advance();
	}
	else {
		std::cout << "Excepted '('"<<'\n';
		return;
	}
	compileExpression();
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ')'){
		tok.advance();
	}
	else {
		std::cout << "Excepted ')'"<<'\n';
		return;
	}
	vmw.writeArithmetic(Command::NOT); //not
	//if-goto label2
	vmw.writeIfGoto(label2);
	
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '{'){
		tok.advance();
	}
	else {
		std::cout << "Excepted '{'"<<'\n';
		return;
	}
	
	compileStatements();
	
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '}'){
		tok.advance();
		//goto label1
		vmw.writeGoto(label1);
	}
	else {
		std::cout << "Excepted '}'"<<'\n';
		return;
	}
	//label2
	vmw.writeLabel(label2);
}


void CompilationEngine::compileDo(){ 
	if (tok.keyWord() == DO){
		tok.advance();
	}
	compileTerm();
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ';'){
		vmw.writePop(Segment::TEMP,0);
		tok.advance();
	}
	else {
		std::cout << "Excepted ';'"<<'\n';
		return;
	}
}


void CompilationEngine::compileReturn(){
	if (tok.keyWord() == RETURN){
		tok.advance();
	}
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ';'){
		vmw.writePush(Segment::CONSTANT,0);
		vmw.writeReturn();
		tok.advance();
		return;
	}
	else compileExpression();
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ';'){
		vmw.writeReturn();
		tok.advance();
	}
	else {
		std::cout << "Excepted ';'"<<'\n';
		return;
	}
}

std::map<char,Command> Op2Cmd {{'+',Command::ADD},{'-',Command::SUB},{'=',Command::EQ},{'>',Command::GT},
								{'<',Command::LT},{'&',Command::AND},{'|',Command::OR}};

void CompilationEngine::compileExpression(){
	compileTerm();
	while (true){
		if (isOp()){
			//store op in somewhere
			auto Op = tok.symbol();
			tok.advance();
			compileTerm();
			//get op,complete postfix
			switch (Op) {
			case '+': case '-': case '=': case '>': case '<': case '&': case '|':   
				vmw.writeArithmetic(Op2Cmd[Op]);
				break;
			case '*':
				vmw.writeCall("Math.multiply",2);
				break;
			case '/':
				vmw.writeCall("Math.divide",2);
				break;
			default:
				std::cout << "Undefined binary operator!"<<'\n';
			}
		}
		else break;
	}
}

bool CompilationEngine::isKwdConst(){
	return tok.keyWord() == KTRUE || tok.keyWord() == KFALSE || 
		tok.keyWord() == KNULL || tok.keyWord() == THIS;
}


void CompilationEngine::compileTerm(){
	if (tok.tokenType() == TokType::INT_CONST){
		vmw.writePush(Segment::CONSTANT,tok.intVal());
		tok.advance();
	}
	else if (tok.tokenType() == TokType::STRING_CONST){
		auto str = tok.stringVal();
		vmw.writePush(Segment::CONSTANT,str.size());
		vmw.writeCall("String.new",1);
		for (auto& c : str){
			vmw.writePush(Segment::CONSTANT,static_cast<int>(c));
			vmw.writeCall("String.appendChar",2);
		}
		tok.advance();
		return;
	}
	else if (isKwdConst()){
		if (tok.keyWord() == KNULL || tok.keyWord() == KFALSE)
			vmw.writePush(Segment::CONSTANT,0);
		else if (tok.keyWord() == KTRUE){
			vmw.writePush(Segment::CONSTANT,0);
			vmw.writeArithmetic(Command::NOT);
		}
		else if (tok.keyWord() == THIS)
			vmw.writePush(Segment::POINTER,0);
		tok.advance();
	}
	else if (tok.tokenType() == TokType::IDENTIFIER){
		auto varName = tok.getCurToken().TokNm;
		Value v = findInST();
		tok.advance();
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '['){  //arr[i]
			if (v.index == -1){
				std::cout << varName <<" undefined!"<<'\n';
				return;
			}
			tok.advance();
			compileExpression();
			vmw.writePush(Kind2Seg[v.kind],v.index);
			if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ']'){
				//arr+i
				vmw.writeArithmetic(Command::ADD);
				//pop pointer 1
				vmw.writePop(Segment::POINTER,1);
				//push that 0
				vmw.writePush(Segment::THAT,0);
				tok.advance();
				return;
			}
		}
		else if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '('){ //FunName(explist)
			tok.advance();
			vmw.writePush(Segment::POINTER,0);
			int args = compileExpressionList();
			//call FunName
			vmw.writeCall(classname +"."+varName,args+1);
			if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ')'){
				tok.advance();
				return;
			}

		}
		else if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '.'){//varName.FunName(explist)
			tok.advance();
			std::string fn,totalname;
			if (tok.tokenType() == TokType::IDENTIFIER){
				fn = tok.getCurToken().TokNm;
				tok.advance();
			}
			if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '('){
				tok.advance();
			}
			if (v.index == -1){
				totalname = varName + "." + fn;
				int args = compileExpressionList();
				//call totalname
				vmw.writeCall(totalname,args);
			}
			else {
				totalname = v.type + "." + fn;
				vmw.writePush(Kind2Seg[v.kind],v.index);
				int args = compileExpressionList();
				//call totalname
				vmw.writeCall(totalname,args+1);
			}
			if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ')'){
				tok.advance();
				return;
			} 
		}
		else {
			if (v.index == -1){
				std::cout << varName <<" undefined!"<<'\n';
				return;
			}
			vmw.writePush(Kind2Seg[v.kind],v.index); //varName
		}
	}
	else if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '('){ //(exp)
		tok.advance();
		compileExpression();
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ')'){
			tok.advance();
		}
		else return;
	}
	else if (isunaryOp()){ //~|- exp
		auto unaryOp = tok.symbol();
		tok.advance();
		compileTerm();
		//get unary
		if (unaryOp == '-') 
			vmw.writeArithmetic(Command::NEG);
		else if (unaryOp == '~')
			vmw.writeArithmetic(Command::NOT);
	}
}


int CompilationEngine::compileExpressionList(){
	int count = 0;
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ')'){
		return 0;
	}
	compileExpression();
	count++;
	while (true){
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ','){
			tok.advance();
			compileExpression();
			count++;
		}
		else break;
	}
	return count;
}


void CompilationEngine::printVMCode(){
	compileClass();
}



