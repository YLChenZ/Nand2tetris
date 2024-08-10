#include "CompilationEngine.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Method: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    CompilationEngine CE(filename);
	
	CE.printXML();

    return 0;
}
