#include "parser.h"
#include "code.h"
#include "symboltable.h"

int main() {
    std::shared_ptr symtab = std::make_shared<SymbolTable>();
    Parser parser1("MaxL.asm", 1, symtab);

    Parser parser2("MaxL.asm", 2, parser1.getsymtab());

    parser1.PrintInstrs("MaxL.pass1");
    parser2.PrintInstrs("MaxL.pass2");

    //parser2.PrintSymTab("MaxL.sb");

    Code code("MaxL.asm",parser2.getsymtab());

    //code.getParser().PrintInstrs();
    //code.getParser().PrintSymTab();

    code.PrintBinCode("MaxL.hack");

    return 0;
}

