#include "codewriter.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Method: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    CodeWriter cw = CodeWriter(filename);
    cw.Vm2Asm();

    return 0;
}
