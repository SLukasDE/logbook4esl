#include "logbook4esl/Example04.h"
#include "logbook4esl/Logger.h"

#include <logbook4esl/logging/Logging.h>

#include <common4esl/logging/DefaultLayout.h>
#include <common4esl/logging/OStreamAppender.h>
#include <common4esl/logging/MemBufferAppender.h>

#include <esl/logging/Appender.h>
#include <esl/logging/Layout.h>
#include <esl/logging/Logging.h>

#include <iostream>
#include <sstream>

namespace logbook4esl {
inline namespace v1_6 {

namespace {
/* creating a logger class */
Logger logger("logbook4esl::Example04");
}

void Example04::run() {
	std::unique_ptr<esl::logging::Logging> aLogging(new logbook4esl::logging::Logging({}));
	esl::logging::Logging::init(std::move(aLogging));

	{
		/* each appender has a layout to specify which columns we want to see.
		 * Let's play a little bit with this feature ... */
		std::unique_ptr<esl::logging::Layout> defaultLayout(new common4esl::logging::DefaultLayout({
			{"show-file", "true"},
			{"show-line-no", "true"}
		}));
		esl::logging::Logging::get()->addLayout("defaultLayout", std::move(defaultLayout));
	}

	{
		std::unique_ptr<esl::logging::Appender> appender(new common4esl::logging::OStreamAppender({
			{"trace", "out"},
			{"debug", "out"},
			{"info", "out"},
			{"warn", "out"},
			{"error", "out"}
		}));
		appender->setRecordLevel(esl::logging::Appender::RecordLevel::SELECTED);
		esl::logging::Logging::get()->addAppender("ostream", "defaultLayout", std::move(appender));
	}

	{
		std::unique_ptr<esl::logging::Appender> appender(new common4esl::logging::MemBufferAppender({
			{"max-lines", "100"},
			{"max-columns", "100"}
		}));
		appender->setRecordLevel(esl::logging::Appender::RecordLevel::ALL);
		esl::logging::Logging::get()->addAppender("mem-buffer", "defaultLayout", std::move(appender));
	}

	esl::logging::Logging::get()->setLevel(esl::logging::Level::WARN, "*");


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
	std::stringstream sstream;
	esl::logging::Logging::get()->flush(&sstream);
	std::cout << "Replay: " << sstream.str() << "\n";
}

} /* inline namespace v1_6 */
} /* namespace logbook4esl */
