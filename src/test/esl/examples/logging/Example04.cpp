#include <esl/examples/logging/Example04.h>
#include <exception>

namespace esl {
namespace examples {
namespace logging {

float Example04::divide(float a, float b) {
	logger.debug(__func__, __FILE__, __LINE__) << "divide " << a << " by " << b << "!\n";

	if(b == 0) {
		logger.error(this, __func__, __FILE__, __LINE__) << "cannot divide " << a << " by " << b << "!\n";
		throw std::runtime_error("division by zero");
	}

	return a/b;
}


void example04() {
	/* Now the initialization has been moved outside of this example. See main-function for this. */

	/* Sometimes you don't want to put a logger to every class or you cannot access it.
	 * In this case it is helpful to have a global logger at least for a namespace that you can use.
	 * Here we did not specify a separate logger for this class.
	 * But we specified a global logger in file "esl/examples/Logger.h" for namespace "esl::examples".
	 * We still can use the variable name "logger" and it gets resolved by C++ way to resolve symbols.
	 */

	/* now let's test it! */
	Example04 e;
	try {
		e.divide(5, 5);
		e.divide(5, 0);
	}
	catch(...) {
		/* you will see in Logger.cpp that we used a second appender (MemBuffer), that catches all log messages,
		 * even if the log level is set to silent. We can use this appender to make a replay of all log messages.
		 */
		loggerReplay();
	}
}

} /* namespace logging */
} /* namespace examples */
} /* namespace esl */
