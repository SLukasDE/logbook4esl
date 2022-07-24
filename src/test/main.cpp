#include <esl/logging/Appender.h>
#include <esl/logging/Layout.h>
#include <esl/logging/Logging.h>
#include <esl/logging/Logger.h>
#include <esl/logging/Level.h>
#include <esl/plugin/Registry.h>

#include <logbook4esl/logging/Logging.h>

#include <common4esl/logging/MemBufferAppender.h>
#include <common4esl/logging/OStreamAppender.h>
#include <common4esl/logging/DefaultLayout.h>

#include <iostream>
#include <memory>

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
	esl::plugin::Registry::get().addPlugin<esl::logging::Logging>(
			"eslx/logging/Logging",
			&logbook4esl::logging::Logging::create);

	esl::plugin::Registry::get().addPlugin<esl::logging::Appender>(
			"eslx/logging/MemBufferAppender",
			&common4esl::logging::MemBufferAppender::create);

	esl::plugin::Registry::get().addPlugin<esl::logging::Appender>(
			"eslx/logging/OStreamAppender",
			&common4esl::logging::OStreamAppender::create);

	esl::plugin::Registry::get().addPlugin<esl::logging::Layout>(
			"eslx/logging/DefaultLayout",
			&common4esl::logging::DefaultLayout::create);

//	std::unique_ptr<esl::logging::Logging> aLogging(new logbook4esl::logging::Logging({}));
//	esl::logging::Logging::init(*aLogging);

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
