#include "code.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Method: " << argv[0] << " <filename>" << '\n';
        return 1;
    }

    std::string filename = argv[1];
    std::shared_ptr symtab = std::make_shared<SymbolTable>();
    Parser parser1(filename, 1, symtab);

    Parser parser2(filename, 2, parser1.getsymtab());

    parser1.PrintInstrs();
    parser2.PrintInstrs();

    parser2.PrintSymTab();

    Code code(filename,parser2.getsymtab());

    //code.getParser().PrintInstrs();
    //code.getParser().PrintSymTab();

    code.PrintBinCode();

    return 0;
}
