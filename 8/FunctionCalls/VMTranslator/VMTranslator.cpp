#include "codewriter.h"

namespace fs = std::filesystem;

void processDirectory(const fs::path& dirPath) {
    if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
        std::cerr << "Directory " << dirPath << " does not exist or is not a directory." << '\n';
        return;
    }

    fs::path outputFilePath = dirPath / (dirPath.filename().string() + ".asm");
    std::ofstream outFile(outputFilePath, std::ios_base::trunc);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open output file: " << outputFilePath << '\n';
        return;
    }
    
	outFile	<< "// Bootstrap code" <<'\n'
			<< "@261"<<'\n'
			<< "D=A"<<'\n'
			<< "@SP"<<'\n'
			<< "M=D"<<'\n'
			<< "@Sys.init"<<'\n'
			<< "0;JMP"<<'\n';


    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".vm") {
            CodeWriter cw(entry.path().string(), outFile);
            cw.Vm2Asm();
        }
    }

    outFile.close(); // Close the output file when done
}

void handleInput(const std::string& inputStr) {
    fs::path inputPath(inputStr);

    if (fs::is_directory(inputPath)) {
        processDirectory(inputPath);
    } else if (fs::is_regular_file(inputPath) && inputPath.extension() == ".vm") {
        fs::path outputFilePath = inputPath.parent_path() / (inputPath.stem().string() + ".asm");
        std::ofstream outFile(outputFilePath);
        if (!outFile.is_open()) {
            std::cerr << "Failed to open output file: " << outputFilePath << '\n';
            return;
        }
        CodeWriter cw(inputPath.string(), outFile);
        cw.Vm2Asm();
        outFile.close(); // Close the output file when done
    } else {
        std::cerr << "Input path " << inputPath << " is not a .vm file or directory." << '\n';
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <directory>" << '\n';
        return 1;
    }

    handleInput(argv[1]);

    return 0;
}






