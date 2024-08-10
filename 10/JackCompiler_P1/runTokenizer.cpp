#include "tokenizer.h"




int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Method: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    Tokenizer Tok(filename);
	
	Tok.printTokens();

    return 0;
}
