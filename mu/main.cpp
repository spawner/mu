#include "mu.h"

int main(int argc, char* argv[]) {
	if (argc > 2) {
		std::println("usage: mu [script]");
	}
	else if (argc == 2) {
		Mu::runFile(argv[1]);
	}
	else {
		Mu::runPrompt();
	}
}