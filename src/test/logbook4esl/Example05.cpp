#include "logbook4esl/Example05.h"
#include "logbook4esl/Logger.h"

#include <logbook4esl/monitoring/Logging.h>

#include <esl/monitoring/Logging.h>

namespace logbook4esl {
inline namespace v1_6 {

namespace {
/* creating a logger class */
Logger logger("logbook4esl::Example05");
}

void Example05::run() {
	std::unique_ptr<esl::monitoring::Logging> aLogging(new logbook4esl::monitoring::Logging({}));
	esl::monitoring::Logging::init(std::move(aLogging));
	esl::monitoring::Logging::get()->addFile("logger.xml");

	/* That's it, now you can use it already */
	logger.info << "Hello world!" << std::endl;

	/* Ups, you see nothing.
	 * We have not "initialized" the logging framework.
	 * Let's go to logger example 2 to see how to initialize... */
}

} /* inline namespace v1_6 */
} /* namespace logbook4esl */
