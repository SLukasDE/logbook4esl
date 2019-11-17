#include <esl/examples/logging/Example03.h>
#include <esl/logging/appender/OStream.h>

namespace esl {
namespace examples {
namespace logging {

namespace { /* anonymous namespace */

/* As before we need an appender to see your output.
 *
 * BTW: You don't have to initialize the logger always for each class!
 * Later you will make it once, near to your main()-function.
 */
esl::logging::appender::OStream appenderOstream(std::cout, std::cout, std::cout, std::cout, std::cout);

} /* anonymous namespace */

/* Now we are using a logger within a class. This is where it becomes very interesting for professional usage.
 * First we put an static instance into the private section and initialize it with the full qualified name class name.
 */
esl::logging::Logger Example03::logger = "esl::examples::logger::Example03";

float Example03::divide(float a, float b) {
	if(b != 0) {
		/* here you can see how to tell the logger where we are. This might be very useful to find the location of an error.
		 * You can leave it empty, then you know already that this output occurred in class "esl::examples::logger::Example03".
		 * But now you have much more details to your location.
		 */
		logger.debug(__func__, __FILE__, __LINE__) << "operation possible\n";
		return a/b;
	}

	/* here we put one more parameter to the logger: "this". This can be useful, to know from which instance the error occurred. */
	logger.error(this, __func__, __FILE__, __LINE__) << "cannot divide by zero!\n";
	return 0;
}






void example03() {
	/* Initialize the logger framework */
	esl::logging::Logger::addAppender(appenderOstream);

	/* each appender has a layout to specify which columns we want to see.
	 * Let's play a little bit with this feature ... */
	appenderOstream.getLayout().setShowFile(true);
	appenderOstream.getLayout().setShowLineNo(true);
	appenderOstream.getLayout().setShowAddress(true);
	appenderOstream.getLayout().setShowFunction(true);
	appenderOstream.getLayout().setShowLevel(true);
	appenderOstream.getLayout().setShowThreadNo(true);
	appenderOstream.getLayout().setShowTimestamp(true);
	appenderOstream.getLayout().setShowTypeName(true);

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
	esl::logging::Logger::setLevel(esl::logging::Level::SILENT, "*");
	esl::logging::Logger::setLevel(esl::logging::Level::WARN, "esl::examples::logger::Example03");

	/* now let's test it! */
	Example03 e;
	e.divide(5, 5);
	e.divide(5, 0);
}

} /* namespace logging */
} /* namespace examples */
} /* namespace esl */
