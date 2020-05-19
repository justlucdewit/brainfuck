// 18-5-2020 9:15

#include <cstdint>
#include <fstream>
#include "Brainfuck.h"

std::string readFile(std::string fileName) {
	std::ifstream inFile;
	inFile.open(fileName);
	if (!inFile) {
		std::cout << "[ERROR] was unable to open file: " << fileName;
		exit(1);
	}
	std::string content;
	char x;
	while (inFile >> x) {
		content += x;
	}
	inFile.close();
	return content;
}

int main(int argc, char** argv){
	if (argc > 1) {
		std::string program = readFile(argv[1]);

		runBrainfuck(program);
	}
	else {
		std::cout << "C++ brainfuck interpreter V1.0.1\nCreated by Luc de Wit\nUsage: brainfuck someFile.bf";
	}
	return 0;
}