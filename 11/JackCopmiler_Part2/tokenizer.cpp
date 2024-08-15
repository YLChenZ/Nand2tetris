#include "tokenizer.h"

Tokenizer::Tokenizer(const std::string& filename)
	: filename(filename), curChar(' '), curToken(Token(" ", TokType::UNKNOWN))
{	
	file.open(filename);
	if (!file.is_open()) {
		std::cerr << "Error opening file" << '\n';
		exit(1);
	}
	
	nextChar();
	advance();


	KWDMap = {{"class",CLASS},{"method",METHOD},{"function",FUNCTION},{"constructor",CONSTRUCTOR},{"int",INT},
			  {"boolean",BOOLEAN},{"char",CHAR},{"void",VOID},{"var",VAR},{"static",STATIC},{"field",FIELD},
			  {"let",LET},{"do",DO},{"if",IF},{"else",ELSE},{"while",WHILE},{"return",RETURN},
			  {"true",KTRUE},{"false",KFALSE},{"null",KNULL},{"this",THIS}};
			  
			  
	size_t commapos= filename.find('.');
	std::string outfn;
	if (commapos != std::string::npos){
		outfn = filename.substr(0,commapos) + "Tok.xml";
	}
	
	outfile.open(outfn);
	
	if (!outfile.is_open()) {
		std::cerr << "Error opening file" << '\n';
		exit(1);
	}		  
}

Tokenizer::~Tokenizer(){
	if (file.is_open()){
		file.close();
	}
	if (outfile.is_open()){
		outfile.close();
	}
}

void Tokenizer::nextChar(){
	curChar = file.get();
}


Token Tokenizer::getID(){
	std::string Tok;
	while (isalpha(curChar) || isdigit(curChar)){
			Tok += curChar;
			nextChar();
	}
	if (Tok == "class" || Tok == "method" || Tok == "function" ||
		Tok == "constructor" || Tok == "int" || Tok == "boolean" ||
		Tok == "char" || Tok == "void" || Tok == "var" || 
		Tok == "static" || Tok == "field" || Tok == "do" || 
		Tok == "if" || Tok == "else" || Tok == "while" || 
		Tok == "return" || Tok == "true" || Tok == "false" || 
		Tok == "null" || Tok == "this" || Tok == "let"){
			return Token(Tok,TokType::KEYWORD);
		}
		else 
			return Token(Tok,TokType::IDENTIFIER);
	//nextChar();
}
Token Tokenizer::getNum(){
	std::string res;
	
	while (isdigit(curChar)){
			res += curChar;
			nextChar();
	}
	//nextChar();
	
	return Token(res,TokType::INT_CONST);
}


Token Tokenizer::getStr(){
	std::string res;
	
	while (true){
		res += curChar;
		nextChar();
		if (curChar == '\"'){
			break;
		}
	}	
	
	res += curChar;
	nextChar();
	return Token(res,TokType::STRING_CONST);
}

bool Tokenizer::isSymbol(char c){
	return c == '{' || c == '}' || c == '(' || c == ')' || 
		   c == '[' || c == ']' || c == '.' || c == ',' || 
		   c == ';' || c == '+' || c == '-' || c == '*' || 
		   c == '/' || c == '&' || c == '|' || c == '<' || 
		   c == '>' || c == '=' || c == '~';
}




void Tokenizer::skipComment(){
	if (curChar == '/'){
		nextChar();
		if (curChar == '/'){
			while (curChar != '\n' && curChar != '\r' && curChar != EOF)
				nextChar();
			nextChar();
		}
		else if (curChar == '*'){
			while (true){
				nextChar();
				if (curChar == '*'){
					nextChar();
					if (curChar == '/')
						break;
				}
			}
			nextChar();
		}
	}
}


void Tokenizer::skipSpace(){
	while(isspace(curChar) || curChar == '\r' || curChar == '\n')
		nextChar();
}

Token Tokenizer::getToken(){
	
	skipSpace();
	
	if (curChar == '/'){
		nextChar();
		if (curChar == '/' || curChar == '*'){
			file.unget();
			file.unget();
			nextChar();
			skipComment();
			return getToken();
		}
		else {
			file.unget();
			file.unget();
			nextChar();
			if (curChar == '/'){
				char c = curChar;
				nextChar();
				return Token(std::string(1,c),TokType::SYMBOL);
			}
		}
	}
	if (isalpha(curChar)){
		return getID();
	}
	
	if (isSymbol(curChar)){
		char symbol = curChar;
		nextChar();
		return Token(std::string(1, symbol),TokType::SYMBOL);
	}
		
	
	if (isdigit(curChar)){
		return getNum();
	}
	
	if (curChar == '\"')
		return getStr();
	
	
	if (curChar == EOF)
		return Token("EOF",TokType::UNKNOWN);
	
	std::string str(1, curChar);
	nextChar();

	return Token(str,TokType::UNKNOWN);
}



bool Tokenizer::hasMoreTokens(){
	return curToken.TokNm != "EOF";
}

void Tokenizer::advance(){
	if (hasMoreTokens())
		curToken = getToken();
}

TokType Tokenizer::tokenType(){
	return curToken.TokTy;
}



KWD Tokenizer::keyWord(){
	if (tokenType() == TokType::KEYWORD)
		return KWDMap[curToken.TokNm];
	else 
		return UNKNOWN;
}


char Tokenizer::symbol(){
	if (tokenType() == TokType::SYMBOL)
		return curToken.TokNm[0];
}


std::string Tokenizer::identifier(){
	if (tokenType() == TokType::IDENTIFIER)
		return curToken.TokNm;
}


int Tokenizer::intVal(){
	if (tokenType() == TokType::INT_CONST)
		return std::stoi(curToken.TokNm);
}


std::string Tokenizer::stringVal(){
	if (tokenType() == TokType::STRING_CONST)
		return curToken.TokNm.substr(1,curToken.TokNm.size()-2);
}


Token Tokenizer::getCurToken(){
	return curToken;
}

void Tokenizer::printSingleToken(){
	TokType tt = tokenType();
	if (tt == TokType::KEYWORD)
		outfile << "<keyword> " << curToken.TokNm << " </keyword>"<<'\n';
	if (tt == TokType::SYMBOL)
		outfile << "<symbol> " << curToken.TokNm << " </symbol>"<<'\n';
	if (tt == TokType::IDENTIFIER)
		outfile << "<identifier> " << curToken.TokNm << " </identifier>"<<'\n';
	if (tt == TokType::INT_CONST)
		outfile << "<integerConstant> " << curToken.TokNm << " </integerConstant>"<<'\n';
	if (tt == TokType::STRING_CONST)
		outfile << "<stringConstant> " << stringVal() << " </stringConstant>"<<'\n';
}

void Tokenizer::printTokens(){
	outfile << "<tokens> "<<'\n';
	while(curToken.TokNm != "EOF"){

		printSingleToken();
		advance();
	}
	outfile << "</tokens> "<<'\n';
		
}
