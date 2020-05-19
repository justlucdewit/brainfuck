#include "Brainfuck.h"
#include "brainVM.h"

void runBrainfuck(const std::string& program) {
	brainVM VM(program);
	VM.optimize();
	VM.execute();
}