#include "logbook4esl/Example02.h"
#include "logbook4esl/Logger.h"

#include <esl/monitoring/LogbookLogging.h>
#include <esl/monitoring/OStreamAppender.h>
#include <esl/monitoring/SimpleLayout.h>
#include <esl/plugin/Registry.h>

namespace logbook4esl {
inline namespace v1_6 {

namespace {
/* creating a logger class */
Logger logger("logbook4esl::Example02");
}

void Example02::run() {
	esl::plugin::Registry& registry(esl::plugin::Registry::get());
	{
		auto logging = esl::monitoring::LogbookLogging::createNative();

		auto layout = esl::monitoring::SimpleLayout::create({});
		logging->addLayout("defaultLayout", std::move(layout));

		auto appender = esl::monitoring::OStreamAppender::create({
			{"trace", "out"},
			{"debug", "out"},
			{"info", "out"},
			{"warn", "out"},
			{"error", "out"}
		});
		logging->addAppender("", "defaultLayout", std::move(appender));

		registry.setObject(std::move(logging));
	}


	/* this function is similar to example01(). But we make a little bit more.
	 * First, we make our logger instance static. It is not necessary to make a new instance every time we call a function.
	 * Second, we give this instance a name.
	 *   Typically you specify your full qualifies class name if you put a logger instance inside your class
	 *   or your full qualified namespace if you use a global available logger for a namespace.
	 */


	/* Now you will see your output */
	logger.info << "Hello again!" << std::endl;

	/* the output might be much more specific as you expected. The details depend on your appender you are using and how you initialized it */
}

} /* inline namespace v1_6 */
} /* namespace logbook4esl */
