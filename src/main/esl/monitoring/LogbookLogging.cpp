#include <esl/monitoring/LogbookLogging.h>

#include <logbook4esl/monitoring/Logging.h>

#include <esl/system/Stacktrace.h>

#include <stdexcept>

namespace esl {
inline namespace v1_6 {
namespace monitoring {

LogbookLogging::Settings::Settings(const std::vector<std::pair<std::string, std::string>>& settings) {
    for(const auto& setting : settings) {
        throw esl::system::Stacktrace::add(std::runtime_error("unknown attribute '\"" + setting.first + "\"'."));
    }
}

LogbookLogging::LogbookLogging(const Settings& settings)
: logging(new logbook4esl::monitoring::Logging(Settings()))
{ }

std::unique_ptr<Logging> LogbookLogging::create(const std::vector<std::pair<std::string, std::string>>& settings) {
	return std::unique_ptr<Logging>(new LogbookLogging(Settings(settings)));
}

std::unique_ptr<Logging> LogbookLogging::createNative(const Settings& settings) {
	return std::unique_ptr<Logging>(new logbook4esl::monitoring::Logging(settings));
}

void LogbookLogging::setUnblocked(bool isUnblocked) {
	logging->setUnblocked(isUnblocked);
}

void LogbookLogging::setLevel(Streams::Level logLevel, const std::string& typeName) {
	logging->setLevel(logLevel, typeName);
}

bool LogbookLogging::isEnabled(const char* typeName, Streams::Level level) {
	return logging->isEnabled(typeName, level);
}

std::unique_ptr<OStream> LogbookLogging::createOStream(const Streams::Location& location) {
	return logging->createOStream(location);
}

unsigned int LogbookLogging::getThreadNo(std::thread::id threadId) {
	return logging->getThreadNo(threadId);
}

void LogbookLogging::flush(std::ostream* oStream) {
	logging->flush(oStream);
}

void LogbookLogging::addData(const std::string& configuration) {
	logging->addData(configuration);
}

void LogbookLogging::addFile(const boost::filesystem::path& filename) {
	logging->addFile(filename);
}

void LogbookLogging::addLayout(const std::string& id, std::unique_ptr<Layout> layout) {
	logging->addLayout(id, std::move(layout));
}

void LogbookLogging::addAppender(const std::string& name, const std::string& layoutRefId, std::unique_ptr<Appender> appender) {
	logging->addAppender(name, layoutRefId, std::move(appender));
}

} /* namespace monitoring */
} /* inline namespace v1_6 */
} /* namespace esl */
