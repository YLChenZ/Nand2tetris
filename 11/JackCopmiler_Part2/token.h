#ifndef JACK_COMPILER_TOKEN
#define JACK_COMPILER_TOKEN

#include <string>

enum class TokType{
	KEYWORD=-1,
	SYMBOL=-2,
	IDENTIFIER=-3,
	INT_CONST=-4,
	STRING_CONST=-5,
	UNKNOWN=0
};

class Token{
public:	
	std::string TokNm;
	TokType TokTy;

	Token(const std::string& tokname, TokType tt);
};


#endif
