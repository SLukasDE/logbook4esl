#include <esl/Module.h>
#include <logbook4esl/Module.h>
#include <esl/examples/logging/Example01.h>
#include <esl/examples/logging/Example02.h>
#include <esl/examples/logging/Example03.h>
#include <esl/examples/logging/Example04.h>
#include <iostream>
#include <string>
#include <esl/logging/Logger.h>


void printUsage() {
	std::cout << "Possible arguments:\n\n";
	std::cout << "  logging-example01\n";
	std::cout << "  logging-example02\n";
	std::cout << "  logging-example03\n";
	std::cout << "  logging-example04\n";
}

int main(int argc, const char *argv[]) {
	esl::addModule(logbook4esl::getModule());
	std::string argument;

	if(argc < 2) {
		argument = "logging-example04";
	}
	else if(argc == 2) {
		argument = argv[1];
	}
	else {
		std::cout << "Wrong number of arguments.\n\n";
		printUsage();
		return -1;
	}

	if(argument == "logging-example01") {
		esl::examples::logging::example01();
	}
	else if(argument == "logging-example02") {
		esl::examples::logging::example02();
	}
	else if(argument == "logging-example03") {
		esl::examples::logging::example03();
	}
	else if(argument == "logging-example04") {
		esl::examples::loggerInitialize();
		esl::examples::logging::example04();
	}
	else {
		std::cout << "unknown argument \"" << argument << "\".\n\n";
		printUsage();
		return -1;
	}
	return 0;
}
