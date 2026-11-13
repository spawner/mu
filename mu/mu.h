#pragma once

#include <iostream>
#include <print>
#include <string>
#include <filesystem>
#include <vector>
#include <fstream>

#include "scanner.h"

class Mu {
public:
	Mu() = default;
	~Mu() = default;

public:
	static void runFile(const std::filesystem::path& path);
	static void runPrompt();

	static void error(std::size_t line, const std::string& message);

private:
	static void run(const std::string& source);
	static void report(std::size_t line, const std::string& where, const std::string& message);

private:
	inline static bool _had_error = false;
};