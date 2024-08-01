#include "codewriter.h"


int main() {

	CodeWriter cw = CodeWriter("PointerTest.vm");
	
	cw.Vm2Asm();

        return 0;
}

/*

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Method: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    CodeWriter cw = CodeWriter(filename);
    codeWriter.Vm2Asm();

    return 0;
}

*/
