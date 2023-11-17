#include "logbook4esl/Example03.h"
#include "logbook4esl/Logger.h"

#include <logbook4esl/monitoring/Logging.h>

#include <common4esl/monitoring/DefaultLayout.h>
#include <common4esl/monitoring/OStreamAppender.h>

#include <esl/monitoring/Appender.h>
#include <esl/monitoring/Layout.h>
#include <esl/monitoring/Logging.h>

namespace logbook4esl {
inline namespace v1_6 {

namespace {
/* creating a logger class */
Logger logger("logbook4esl::Example03");
}

void Example03::run() {
	std::unique_ptr<esl::monitoring::Logging> aLogging(new logbook4esl::monitoring::Logging({}));
	esl::monitoring::Logging::init(std::move(aLogging));

	{
		/* each appender has a layout to specify which columns we want to see.
		 * Let's play a little bit with this feature ... */
		std::unique_ptr<esl::monitoring::Layout> defaultLayout(new common4esl::monitoring::DefaultLayout({
			{"show-timestamp", "true"},
			{"show-level", "true"},
			{"show-type-name", "true"},
			{"show-address", "true"},
			{"show-file", "true"},
			{"show-function", "true"},
			{"show-line-no", "true"},
			{"show-thread-no", "true"}
		}));
		esl::monitoring::Logging::get()->addLayout("defaultLayout", std::move(defaultLayout));
	}

	{
		std::unique_ptr<esl::monitoring::Appender> oStreamAppender(new common4esl::monitoring::OStreamAppender({
			{"trace", "out"},
			{"debug", "out"},
			{"info", "out"},
			{"warn", "out"},
			{"error", "out"}
		}));
		esl::monitoring::Logging::get()->addAppender("", "defaultLayout", std::move(oStreamAppender));
	}

	/* Let's do some more initialization...
	 * Most time we are not interested into all log messages of your application.
	 * There are 2 dimensions to tell the logger what messages you want to see.
	 *
	 * The first dimension is the log-level.
	 * TRACE is the highest level of the logger and it includes all log messages.
	 * SILENT is the lowest level of the logger and as you can imagine it includes no log messages.
	 * This is the hierarchy of all levels: TRACE > DEBUG > INFO > WARN > ERROR > SILENT
	 *
	 * The second dimension is the "class path" where you want to specify a particular log level.
	 * It is possible to specify for each path and it's sub paths a different log level.
	 * Here we want to make all log messages silent but for class Example03 we want to get all log messages up to "WARN"-level.
	 */
	esl::monitoring::Logging::get()->setLevel(esl::monitoring::Level::SILENT, "*");
	esl::monitoring::Logging::get()->setLevel(esl::monitoring::Level::WARN, "logbook4esl::Example03");

	Example03 e;
	e.divide(5, 5);
	e.divide(5, 0);
}

float Example03::divide(float a, float b) {
	if(b != 0) {
		/* here you can see how to tell the logger where we are. This might be very useful to find the location of an error.
		 * You can leave it empty, then you know already that this output occurred in class "esl::examples::logger::Example03".
		 * But now you have much more details to your location.
		 */
		ESL__LOGGER_DEBUG_THIS("operation possible\n");
		//logger.debug(__func__, __FILE__, __LINE__) << "operation possible\n";
		return a/b;
	}

	/* here we put one more parameter to the logger: "this". This can be useful, to know from which instance the error occurred. */
	logger.error(this, __func__, __FILE__, __LINE__) << "cannot divide by zero!\n";
	return 0;
}

} /* inline namespace v1_6 */
} /* namespace logbook4esl */
