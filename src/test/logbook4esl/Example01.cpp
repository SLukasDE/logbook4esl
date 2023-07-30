#include "logbook4esl/Example01.h"
#include "logbook4esl/Logger.h"

namespace logbook4esl {
inline namespace v1_6 {

namespace {
/* creating a logger class */
Logger logger("logbook4esl::Example01");
}

void Example01::run() {
	/* That's it, now you can use it already */
	logger.info << "Hello world!" << std::endl;

	/* Ups, you see nothing.
	 * We have not "initialized" the logging framework.
	 * Let's go to logger example 2 to see how to initialize... */
}

} /* inline namespace v1_6 */
} /* namespace logbook4esl */
