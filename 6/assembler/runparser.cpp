#include "parser.h"


int main() {
    	std::shared_ptr symtab = std::make_shared<SymbolTable>();
	Parser parser1("Rect.asm",1,symtab);
	
	Parser parser2("Rect.asm",2,parser1.getsymtab());
	
	parser1.PrintInstrs("pass1.out");
	parser2.PrintInstrs("pass2.out");

	
	parser2.PrintSymTab("output.sb");

        return 0;
}
