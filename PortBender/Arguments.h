#pragma once

#include <string>
#include <vector>

class Arguments {
public:
	Arguments(std::vector<std::string> args);
	std::string Action;
	short FakeDstPort;
	short RedirectPort;
	std::string Password;
};