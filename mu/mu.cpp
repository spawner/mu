#include "mu.h"

void Mu::runFile(const std::filesystem::path& path) {
	std::ifstream file(path, std::ios::binary);
	if (!file) {
		return;
	}

	std::uintmax_t size = std::filesystem::file_size(path);
	std::string program(size, '\0');

	file.read(program.data(), size);
	std::println("program: {} successfully read", program);

	run(program);

	if (_had_error) { exit(-1); }
}

void Mu::runPrompt() {
	std::string line;
	while (std::getline(std::cin, line)) {
		run(line);
		_had_error = false;
	}
}

void Mu::error(std::size_t line, const std::string& message) {
	report(line, "", message);
}

void Mu::run(const std::string& source) {
	Scanner scanner(source);

	std::vector<Token> tokens = scanner.scanTokens();

	for (const auto& token : tokens) {
		std::println("{}", token.toString());
	}
}

void Mu::report(std::size_t line, const std::string& where, const std::string& message) {
	std::println("line {}, error {} {}", line, where, message);
	_had_error = true;
}
