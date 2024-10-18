#include "logbook4esl/Example04.h"
#include "logbook4esl/Logger.h"

#include <logbook4esl/monitoring/Logging.h>

#include <esl/monitoring/MemBufferAppender.h>
#include <esl/monitoring/OStreamAppender.h>
#include <esl/monitoring/LogbookLogging.h>
#include <esl/monitoring/Logging.h>
#include <esl/monitoring/SimpleLayout.h>
#include <esl/plugin/Registry.h>

#include <iostream>
#include <sstream>

namespace logbook4esl {
inline namespace v1_6 {

namespace {
/* creating a logger class */
Logger logger("logbook4esl::Example04");
}

void Example04::run() {
	esl::plugin::Registry& registry(esl::plugin::Registry::get());
	{
		auto logging = esl::monitoring::LogbookLogging::createNative();

		/* each appender has a layout to specify which columns we want to see.
		 * Let's play a little bit with this feature ... */
		auto layout = esl::monitoring::SimpleLayout::create({
			{"show-file", "true"},
			{"show-line-no", "true"}
		});
		logging->addLayout("defaultLayout", std::move(layout));

		{
			auto appender = esl::monitoring::OStreamAppender::create({
				{"trace", "out"},
				{"debug", "out"},
				{"info", "out"},
				{"warn", "out"},
				{"error", "out"}
			});
			logging->addAppender("ostream", "defaultLayout", std::move(appender));
		}

		{
			auto appender = esl::monitoring::MemBufferAppender::create({
				{"max-lines", "100"},
				{"max-columns", "100"}
			});
			logging->addAppender("mem-buffer", "defaultLayout", std::move(appender));
		}

		logging->setLevel(esl::monitoring::Streams::Level::warn, "*");

		registry.setObject(std::move(logging));
	}


	try {
		Example04 e;
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

float Example04::divide(float a, float b) {
	logger.debug(__func__, __FILE__, __LINE__) << "divide " << a << " by " << b << "!\n";

	if(b == 0) {
		logger.error(this, __func__, __FILE__, __LINE__) << "cannot divide " << a << " by " << b << "!\n";
		throw std::runtime_error("division by zero");
	}

	return a/b;
}

void Example04::loggerReplay() {
	auto logging = esl::plugin::Registry::get().findObject<esl::monitoring::Logging>();
	if(logging) {
		std::stringstream sstream;
		logging->flush(&sstream);
		std::cout << "Replay: " << sstream.str() << "\n";
	}
}

} /* inline namespace v1_6 */
} /* namespace logbook4esl */
