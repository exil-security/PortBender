#include <iostream>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN

#include "Arguments.h"
#include "PortBender.h" 

int main(int argc, char* argv[]) {
	try {
		if (argc < 2) {
			throw std::invalid_argument("Missing required arguments");
		}

		Arguments args = Arguments(std::vector<std::string>(argv + 1, argv + argc));

		std::cout << " _______                        __      _______                             __                           " << std::endl;
		std::cout << "/       \\                      /  |    /       \\                           /  |                        " << std::endl;
		std::cout << "$$$$$$$  | ______    ______   _$$ |_   $$$$$$$  |  ______   _______    ____$$ |  ______    ______        " << std::endl;
		std::cout << "$$ |__$$ |/      \\  /      \\ / $$   |  $$ |__$$ | /      \\ /       \\  /    $$ | /      \\  /      \\ " << std::endl;
		std::cout << "$$    $$//$$$$$$  |/$$$$$$  |$$$$$$/   $$    $$< /$$$$$$  |$$$$$$$  |/$$$$$$$ |/$$$$$$  |/$$$$$$  |      " << std::endl;
		std::cout << "$$$$$$$/ $$ |  $$ |$$ |  $$/   $$ | __ $$$$$$$  |$$    $$ |$$ |  $$ |$$ |  $$ |$$    $$ |$$ |  $$/       " << std::endl;
		std::cout << "$$ |     $$ \\__$$ |$$ |        $$ |/  |$$ |__$$ |$$$$$$$$/ $$ |  $$ |$$ \\__$$ |$$$$$$$$/ $$ |          " << std::endl;
		std::cout << "$$ |     $$    $$/ $$ |        $$  $$/ $$    $$/ $$       |$$ |  $$ |$$    $$ |$$       |$$ |            " << std::endl;
		std::cout << "$$/       $$$$$$/  $$/          $$$$/  $$$$$$$/   $$$$$$$/ $$/   $$/  $$$$$$$/  $$$$$$$/ $$/             " << std::endl;
		std::cout << "                                                                                                         " << std::endl;

		if (args.Action == "backdoor") {
			std::cout << "Initializing PortBender in backdoor mode" << std::endl;
			PortBender backdoor = PortBender(args.FakeDstPort, args.RedirectPort, args.Password);
			backdoor.Start();
		}
		else if (args.Action == "redirect") {
			std::cout << "Initializing PortBender in redirector mode" << std::endl;
			PortBender redirector = PortBender(args.FakeDstPort, args.RedirectPort);
			redirector.Start();
		}
	}
	catch (const std::invalid_argument&) {
		std::cout << "Redirect Usage: PortBender redirect SrcPort DstPort" << std::endl;
		std::cout << "Backdoor Usage: PortBender backdoor SrcPort DstPort password" << std::endl;
		std::cout << "Example:" << std::endl;
		std::cout << "\tPortBender redirect 445 8445" << std::endl;
		std::cout << "\tPortBender backdoor 443 3389 portbender.antihacker" << std::endl;
		std::cout << std::flush;
		return -1;
	}
	return 0;
}
