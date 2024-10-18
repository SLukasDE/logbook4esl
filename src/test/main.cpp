#include <esl/plugin/Registry.h>

#include "logbook4esl/Example01.h"
#include "logbook4esl/Example02.h"
#include "logbook4esl/Example03.h"
#include "logbook4esl/Example04.h"
#include "logbook4esl/Example05.h"

#include <iostream>
#include <string>


void printUsage() {
	std::cout << "Possible arguments:\n\n";
	std::cout << "  logging-example01\n";
	std::cout << "  logging-example02\n";
	std::cout << "  logging-example03\n";
	std::cout << "  logging-example04\n";
	std::cout << "  logging-example05\n";
}

int main(int argc, const char *argv[]) {
	std::string argument;
	if(argc == 2) {
		argument = argv[1];
	}
	else {
		std::cout << "Wrong number of arguments.\n\n";
		printUsage();
		return -1;
	}

	struct Cleanup {
		~Cleanup() {
			esl::plugin::Registry::get().cleanup();
		}
	} cleanup;

	if(argument == "logging-example01") {
		logbook4esl::Example01::run();
	}
	else if(argument == "logging-example02") {
		logbook4esl::Example02::run();
	}
	else if(argument == "logging-example03") {
		logbook4esl::Example03::run();
	}
	else if(argument == "logging-example04") {
		logbook4esl::Example04::run();
	}
	else if(argument == "logging-example05") {
		logbook4esl::Example05::run();
	}
	else {
		std::cout << "unknown argument \"" << argument << "\".\n\n";
		printUsage();
		return -1;
	}

	return 0;
}
