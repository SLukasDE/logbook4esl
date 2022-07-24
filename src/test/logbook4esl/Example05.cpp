#include "logbook4esl/Example05.h"
#include "logbook4esl/Logger.h"

#include <logbook4esl/logging/Logging.h>

#include <esl/logging/Logging.h>

namespace logbook4esl {

namespace {
/* creating a logger class */
Logger logger("logbook4esl::Example05");
}

void Example05::run() {
	std::unique_ptr<esl::logging::Logging> aLogging(new logbook4esl::logging::Logging({}));
	esl::logging::Logging::init(std::move(aLogging));
	esl::logging::Logging::get()->addFile("logger.xml");

	/* That's it, now you can use it already */
	logger.info << "Hello world!" << std::endl;

	/* Ups, you see nothing.
	 * We have not "initialized" the logging framework.
	 * Let's go to logger example 2 to see how to initialize... */
}

} /* namespace logbook4esl */
