#include <esl/examples/logging/Example01.h>
#include <esl/logging/Logger.h>

namespace esl {
namespace examples {
namespace logging {

void example01() {
	/* creating a logger class */
	esl::logging::Logger<> logger;

	/* That's it, now you can use it already */
	logger.info << "Hello world!" << std::endl;

	/* Ups, you see nothing.
	 * We have not "initialized" the logging framework.
	 * Let's go to logger example 2 to see how to initialize... */
}

} /* namespace logging */
} /* namespace examples */
} /* namespace esl */
