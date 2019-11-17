#include <esl/examples/Logger.h>
#include <esl/logging/appender/OStream.h>
#include <esl/logging/appender/MemBuffer.h>
#include <esl/logging/Id.h>

#include <vector>
#include <tuple>
#include <iostream>
#include <string>

namespace esl {
namespace examples {

namespace {
esl::logging::appender::OStream appenderOstream(std::cout, std::cout, std::cout, std::cout, std::cout);

/* MemBuffer appender just writes output to a buffer of a fixed number of lines.
 * If number of columns is specified as well the whole memory is allocated at initialization time.
 */
esl::logging::appender::MemBuffer appenderMemBuffer(100);
//esl::logging::appender::MemBuffer appenderMemBuffer(100, 200);
}

esl::logging::Logger logger("esl::examples");

void loggerInitialize() {
	appenderOstream.setRecordLevel();
	appenderOstream.getLayout().setShowFile(true);
	appenderOstream.getLayout().setShowLineNo(true);
	esl::logging::Logger::addAppender(appenderOstream);

	/* Setting record level to ALL to an appender makes him catching all log messages, even if global logging level is set to SILENT */
	appenderMemBuffer.setRecordLevel(esl::logging::Appender::RecordLevel::ALL);
	appenderMemBuffer.getLayout().setShowFile(true);
	appenderMemBuffer.getLayout().setShowLineNo(true);
	esl::logging::Logger::addAppender(appenderMemBuffer);

	/* setting default */
	esl::logging::Logger::setLevel(esl::logging::Level::WARN, "*");

	//esl::logging::Logger::setLevel(esl::logging::Level::INFO, "AAA::BBB::*");
	//esl::logging::Logger::setLevel(esl::logging::Level::DEBUG, "AAA::BBB::CCC::*");
}

void loggerReplay() {
	appenderMemBuffer.getLayout().setShowTypeName(false);
	std::vector<std::tuple<esl::logging::Id, std::string>> buffer = appenderMemBuffer.getBuffer();
	std::cout << "Replay: " << buffer.size() << " elements\n";
	for(const auto& line : buffer) {
		std::cout << appenderMemBuffer.getLayout().makePrefix(std::get<0>(line)) << std::get<1>(line) << "\n";
	}
}


} /* namespace examples */
} /* namespace esl */
