#include "CompilationEngine.h"

CompilationEngine::CompilationEngine(const std::string& filename)
	: tok(Tokenizer(filename))
{
	//tok.advance();
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
}
CompilationEngine::~CompilationEngine(){
	if (out.is_open()){
		out.close();
	}
}


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
	//out << tok.getCurToken().TokNm<<'\n';
	out << "<class>"<<'\n';
	if (tok.keyWord() == CLASS){
		out << "<keyword> " << tok.getCurToken().TokNm << " </keyword>"<<'\n';
		tok.advance();
	}
	else{
		return;
	}
	
	if (tok.tokenType() == TokType::IDENTIFIER){
		out << "<identifier> " << tok.getCurToken().TokNm << " </identifier>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted an identifier in compileClass"<<'\n';
		return;
	}
	if (tok.symbol() == '{'){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
	}
	else{
		out << "Excepted '{'"<<'\n';
		return;
	}
	while (tok.keyWord() == FIELD || tok.keyWord() == STATIC)
		compileClassVarDec();
	while (tok.keyWord() == CONSTRUCTOR || tok.keyWord() == FUNCTION || tok.keyWord() == METHOD)
		compileSubroutine();
	if (tok.symbol() == '}'){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
	}
	else{
		out << "Excepted '}' in compileClass"<<'\n';
		return;
	}
	out << "</class>"<<'\n';
}


void CompilationEngine::compileClassVarDec(){
	out << "<classVarDec>"<<'\n';
	
	if (tok.keyWord() == FIELD || tok.keyWord() == STATIC){	
		out<< "<keyword> " << tok.getCurToken().TokNm << " </keyword>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted 'filed' or 'static' in compileClassVarDec"<<'\n';
		return;
	}
	
	if (isDataType()){
		out << "<keyword> " << tok.getCurToken().TokNm << " </keyword>"<<'\n';
		tok.advance();
	}
	else if (tok.tokenType() == TokType::IDENTIFIER){
		out << "<identifier> " << tok.getCurToken().TokNm << " </identifier>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted a type like 'int' or classname in compileClassVarDec"<<'\n';
		return;
	}
	if (tok.tokenType() == TokType::IDENTIFIER){
		out << "<identifier> " << tok.getCurToken().TokNm << " </identifier>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted an identifier in compileClassVarDec"<<'\n';
		return;
	}
	
	while (true){
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ','){
			out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
			tok.advance();
			if (tok.tokenType() == TokType::IDENTIFIER){
				out << "<identifier> " << tok.getCurToken().TokNm << " </identifier>"<<'\n';
				tok.advance();
			}
			else {
				out << "Excepted an identifier in compileClassVarDec While"<<'\n';
				return;
			}
		}
		else break;
	}
	
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ';'){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted ';' in compileClassVarDec end"<<'\n';
		return;
	}
	out << "</classVarDec>"<<'\n';

}


void CompilationEngine::compileSubroutine(){
	out << "<subroutineDec>"<<'\n';
	if (tok.keyWord() == CONSTRUCTOR || tok.keyWord() == FUNCTION || tok.keyWord() == METHOD){
		out << "<keyword> " << tok.getCurToken().TokNm << " </keyword>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted 'constructor', 'function' or 'method' in compileSubroutine"<<'\n';
		return;
	}
	if (isRetType()){
		out << "<keyword> " << tok.getCurToken().TokNm << " </keyword>"<<'\n';
		tok.advance();
	}
	else if (tok.tokenType() == TokType::IDENTIFIER){
		out << "<identifier> " << tok.identifier() << " </identifier>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted 'int', 'bool', 'char', 'void' or a class name in compileSubroutine retType"<<'\n';
		return;
	}
	
	if (tok.tokenType() == TokType::IDENTIFIER){
		out << "<identifier> " << tok.identifier() << " </identifier>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted an identifier in compileSubroutine name"<<'\n';
		return;
	}
	
	compileParameterList();
	compileSubroutineBody();
	out << "</subroutineDec>"<<'\n';
}


void CompilationEngine::compileParameterList(){
	
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '('){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		out << "<parameterList>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted '(' in compileParameterList"<<'\n';
		return;
	}
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ')'){
		out << "</parameterList>"<<'\n';
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
		return;
	}
	else if (isDataType()){
		out << "<keyword> " << tok.getCurToken().TokNm << " </keyword>"<<'\n';
		tok.advance();
	}
	else if (tok.tokenType() == TokType::IDENTIFIER){
		out << "<identifier> " << tok.identifier() << " </identifier>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted 'int', 'bool', 'char' or a class name in compileParameterList"<<'\n';
		return;
	}
	if (tok.tokenType() == TokType::IDENTIFIER){
		out << "<identifier> " << tok.getCurToken().TokNm << " </identifier>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted an identifier in compileParameterList"<<'\n';
		return;
	}
	
	while (true){
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ','){
			out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
			tok.advance();
			if (isDataType()){
				out << "<keyword> " << tok.getCurToken().TokNm << " </keyword>"<<'\n';
				tok.advance();
			}
			else if (tok.tokenType() == TokType::IDENTIFIER){
				out << "<identifier> " << tok.identifier() << " </identifier>"<<'\n';
				tok.advance();
			}
			else {
				out << "Excepted 'int', 'bool', 'char' or a class name in compileParameterList While"<<'\n';
				return;
			}
			if (tok.tokenType() == TokType::IDENTIFIER){
				out << "<identifier> " << tok.getCurToken().TokNm << " </identifier>"<<'\n';
				tok.advance();
			}
			else {
				out << "Excepted an identifier in compileParameterList While"<<'\n';
				return;
			}
		}
		else break;
	}
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ')'){
		out << "</parameterList>"<<'\n';
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted ')' in in compileParameterList"<<'\n';
		return;
	}
}


void CompilationEngine::compileSubroutineBody(){
	out << "<subroutineBody>"<<'\n';
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '{'){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted '{' in compileSubroutineBody"<<'\n';
		return;
	}
	
	while (tok.keyWord() == VAR)
		compileVarDec();
	compileStatements();
	
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '}'){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted '}' in compileSubroutineBody"<<'\n';
		return;
	}
	out << "</subroutineBody>"<<'\n';
}


void CompilationEngine::compileVarDec(){
	out << "<varDec>"<<'\n';
	if (tok.keyWord() == VAR){
		out << "<keyword> " << tok.getCurToken().TokNm << " </keyword>"<<'\n';
		tok.advance();
	}
	if (isDataType()){
		out << "<keyword> " << tok.getCurToken().TokNm << " </keyword>"<<'\n';
		tok.advance();
	}
	else if (tok.tokenType() == TokType::IDENTIFIER){
		out << "<identifier> " << tok.identifier() << " </identifier>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted 'int', 'bool', 'char' or a class name"<<'\n';
		return;
	}
	if (tok.tokenType() == TokType::IDENTIFIER){
		out << "<identifier> " << tok.getCurToken().TokNm << " </identifier>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted an identifier in compileVarDec"<<'\n';
		return;
	}
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ';'){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		out << "</varDec>"<<'\n';
		tok.advance();
		return;
	}
	while (true){
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ','){
			out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
			tok.advance();

			if (tok.tokenType() == TokType::IDENTIFIER){
				out << "<identifier> " << tok.getCurToken().TokNm << " </identifier>"<<'\n';
				tok.advance();
			}
			else {
				out << "Excepted an identifier in compileVarDec While"<<'\n';
				return;
			}
		}
		else break;
	}
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ';'){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted ';' in compileVarDec"<<'\n';
		return;
	}
	out << "</varDec>"<<'\n';
}


void CompilationEngine::compileStatements(){
	out << "<statements>"<<'\n';
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
	out << "</statements>"<<'\n';
}


void CompilationEngine::compileLet(){
	out << "<letStatement>"<<'\n';
	if (tok.keyWord() == LET){
		out << "<keyword> " << tok.getCurToken().TokNm << " </keyword>"<<'\n';
		tok.advance();
	}
	if (tok.tokenType() == TokType::IDENTIFIER){
		out << "<identifier> " << tok.getCurToken().TokNm << " </identifier>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted an identifier in compileLet"<<'\n';
		return;
	}
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '['){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
		compileExpression();
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ']'){
			out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
			tok.advance();
		}
	}
	
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '='){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted '=' in compileLet"<<'\n';
		return;
	}
	compileExpression();
	
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ';'){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted ';' in compileLet"<<'\n';
		return;
	}
	
	out << "</letStatement>"<<'\n';
}


void CompilationEngine::compileIf(){
	out << "<ifStatement>"<<'\n';
	if (tok.keyWord() == IF){
		out << "<keyword> " << tok.getCurToken().TokNm << " </keyword>"<<'\n';
		tok.advance();
	}
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '('){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted '('"<<'\n';
		return;
	}
	compileExpression();
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ')'){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted ')'"<<'\n';
		return;
	}
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '{'){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted '{'"<<'\n';
		return;
	}
	
	compileStatements();
	
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '}'){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted '}'"<<'\n';
		return;
	}
	if (tok.keyWord() == ELSE){
		out << "<keyword> " << tok.getCurToken().TokNm << " </keyword>"<<'\n';
		tok.advance();
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '{'){
			out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
			tok.advance();
		}
		else {
			out << "Excepted '{'"<<'\n';
			return;
		}
	
		compileStatements();
	
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '}'){
			out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
			tok.advance();
		}
		else {
			out << "Excepted '}'"<<'\n';
			return;
		}
	}
	out << "</ifStatement>"<<'\n';
	return;
}


void CompilationEngine::compileWhile(){
	out << "<whileStatement>"<<'\n';
	if (tok.keyWord() == WHILE){
		out << "<keyword> " << tok.getCurToken().TokNm << " </keyword>"<<'\n';
		tok.advance();
	}
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '('){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted '('"<<'\n';
		return;
	}
	compileExpression();
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ')'){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted ')'"<<'\n';
		return;
	}
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '{'){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted '{'"<<'\n';
		return;
	}
	
	compileStatements();
	
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '}'){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted '}'"<<'\n';
		return;
	}
	out << "</whileStatement>"<<'\n';
}


void CompilationEngine::compileDo(){
	out << "<doStatement>"<<'\n';
	if (tok.keyWord() == DO){
		out << "<keyword> " << tok.getCurToken().TokNm << " </keyword>"<<'\n';
		tok.advance();
	}
	if (tok.tokenType() == TokType::IDENTIFIER){
		out << "<identifier> " << tok.getCurToken().TokNm << " </identifier>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted an identifier in compileDo"<<'\n';
		return;
	}
	
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '('){
		out << "<symbol> " << tok.symbol() << " </symbol>" <<'\n';
		tok.advance();
		compileExpressionList();
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ')'){
			out << "<symbol> " << tok.symbol() << " </symbol>" <<'\n';
			tok.advance();
		}
		else {
			out << "Excepted ')' in compileDo" <<'\n';
			return;
		}
	}
	else if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '.'){
		out << "<symbol> " << tok.symbol() << " </symbol>" <<'\n';
		tok.advance();
		if (tok.tokenType() == TokType::IDENTIFIER){
			out << "<identifier> " << tok.getCurToken().TokNm << " </identifier>"<<'\n';
			tok.advance();
			if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '('){
				out << "<symbol> " << tok.symbol() << " </symbol>" <<'\n';
				tok.advance();
				compileExpressionList();
				if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ')'){
					out << "<symbol> " << tok.symbol() << " </symbol>" <<'\n';
					tok.advance();
				}
			}
		}
		else {
			out << "Excepted an identifier in compileDo"<<'\n';
			return;
		}	
	}
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ';'){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted ';' in compileLet"<<'\n';
		return;
	}
	out << "</doStatement>"<<'\n';
}


void CompilationEngine::compileReturn(){
	out << "<returnStatement>"<<'\n';
	if (tok.keyWord() == RETURN){
		out << "<keyword> " << tok.getCurToken().TokNm << " </keyword>"<<'\n';
		tok.advance();
	}
	
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ';'){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
		out << "</returnStatement>"<<'\n';
		return;
	}
	else compileExpression();
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ';'){
		out << "<symbol> " << tok.symbol() << " </symbol>"<<'\n';
		tok.advance();
	}
	else {
		out << "Excepted ';'"<<'\n';
		return;
	}
	out << "</returnStatement>"<<'\n';
}


void CompilationEngine::compileExpression(){
	out << "<expression>"<<'\n';
	compileTerm();
	while (true){
		if (isOp()){
			out << "<symbol> " << tok.TransForXML(tok.symbol()) << " </symbol>"<<'\n';
			tok.advance();
			compileTerm();
		}
		else break;	
	}
	out << "</expression>"<<'\n';
}

bool CompilationEngine::isKwdConst(){
	return tok.keyWord() == KTRUE || tok.keyWord() == KFALSE || 
		tok.keyWord() == KNULL || tok.keyWord() == THIS;
}


void CompilationEngine::compileTerm(){
	out << "<term> " << '\n';
	if (tok.tokenType() == TokType::INT_CONST){
		out << "<integerConstant> " << tok.intVal() << " </integerConstant>" <<'\n';
		tok.advance();
	}
	else if (tok.tokenType() == TokType::STRING_CONST){
		out << "<stringConstant> " << tok.stringVal() << " </stringConstant>" <<'\n';
		tok.advance();
	}
	else if (isKwdConst()){
		out << "<keyword> " << tok.getCurToken().TokNm << " </keyword>" <<'\n';
		tok.advance();
	}
	else if (tok.tokenType() == TokType::IDENTIFIER){
		out << "<identifier> " << tok.getCurToken().TokNm << " </identifier>"<<'\n';
		tok.advance();
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '['){
			out << "<symbol> " << tok.symbol() << " </symbol>" <<'\n'; 
			tok.advance();
			compileExpression();
			if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ']'){
				out << "<symbol> " << tok.symbol() << " </symbol>" <<'\n';
				tok.advance();
				out << "</term>" <<'\n';
				return;
			}
		}
		else if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '('){
			out << "<symbol> " << tok.symbol() << " </symbol>" <<'\n';
			tok.advance();
			compileExpressionList();
			if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ')'){
				out << "<symbol> " << tok.symbol() << " </symbol>" <<'\n';
				tok.advance();
				out << "</term>" <<'\n';
				return;
			}

		}
		else if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '.'){
			out << "<symbol> " << tok.symbol() << " </symbol>" <<'\n';
			tok.advance();
			if (tok.tokenType() == TokType::IDENTIFIER){
				out << "<identifier> " << tok.getCurToken().TokNm << " </identifier>"<<'\n';
				tok.advance();
			}
			if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '('){
				out << "<symbol> " << tok.symbol() << " </symbol>" <<'\n';
				tok.advance();
			}
			compileExpressionList();
			if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ')'){
				out << "<symbol> " << tok.symbol() << " </symbol>" <<'\n';
				tok.advance();
				out << "</term>" <<'\n';
				return;
			} 
		}
		else {
			out << "</term>" <<'\n';
			return;
		}
	}
	else if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == '('){
		out << "<symbol> " << tok.symbol() << " </symbol>" <<'\n'; 
		tok.advance();
		compileExpression();
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ')'){
			out << "<symbol> " << tok.symbol() << " </symbol>" <<'\n';
			tok.advance();
		}
		else {
			out << "</term>" <<'\n';
			return;
		}
	}
	else if (isunaryOp()){
		out << "<symbol> " << tok.symbol() << " </symbol>" <<'\n';
		tok.advance();
		compileTerm();
	}
	//else return;
	
	out << "</term>" <<'\n';
}


int CompilationEngine::compileExpressionList(){
	int count = 0;
	out << "<expressionList>" <<'\n';
	if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ')'){
		out << "</expressionList>" <<'\n';
		return 0;
	}
	compileExpression();
	count++;
	while (true){
		if (tok.tokenType() == TokType::SYMBOL && tok.symbol() == ','){
			out << "<symbol> " << tok.symbol() << " </symbol>" <<'\n';
			tok.advance();
			compileExpression();
			count++;
		}
		else break;
	}
	out << "</expressionList>" <<'\n';
	return count;
}


void CompilationEngine::printXML(){
	compileClass();
}



