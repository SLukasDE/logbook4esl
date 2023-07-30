/*
MIT License
Copyright (c) 2019-2023 Sven Lukas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <logbook4esl/logging/Logging.h>
#include <logbook4esl/config/Logger.h>

#include <logbook/Logbook.h>
#include <logbook/Level.h>

#include <esl/system/Stacktrace.h>

#include <sstream>
#include <stdexcept>

namespace logbook4esl {
inline namespace v1_6 {
namespace logging {

namespace {
logbook::Level eslLoggingLevel2logbookLevel(esl::logging::Level logLevel) {
	switch(logLevel) {
	case esl::logging::Level::TRACE:
		return logbook::Level::TRACE;
	case esl::logging::Level::DEBUG:
		return logbook::Level::DEBUG;
	case esl::logging::Level::INFO:
		return logbook::Level::INFO;
	case esl::logging::Level::WARN:
		return logbook::Level::WARN;
	case esl::logging::Level::ERROR:
		return logbook::Level::ERROR;
	case esl::logging::Level::SILENT:
		return logbook::Level::SILENT;
	default:
		break;
	}
	throw std::runtime_error("conversion error from esl::logging::Level to logbook::Level");
}

class OStream : public esl::logging::OStream {
public:
	OStream(Logging& aLogging, std::unique_ptr<logbook::Writer> aWriter)
	: logging(aLogging),
	  writer(std::move(aWriter))
	{ }

    std::ostream* getOStream() override {
    	if(writer) {
        	return writer->getOStream();
    	}
    	return nullptr;
    }

    void flush() override {
    	logging.flush(nullptr);
    }

private:
    Logging& logging;
    std::unique_ptr<logbook::Writer> writer;
};

} /* anonymous namespace */

std::unique_ptr<esl::logging::Logging> Logging::create(const std::vector<std::pair<std::string, std::string>>& settings) {
	return std::unique_ptr<esl::logging::Logging>(new Logging(settings));
}

Logging::Logging(const std::vector<std::pair<std::string, std::string>>& settings)
{
    for(const auto& setting : settings) {
        throw esl::system::Stacktrace::add(std::runtime_error("unknown attribute '\"" + setting.first + "\"'."));
    }
}

Logging::~Logging() {
	// Never call a  virtual method from destructor!
	// So, we don't call virtual method flush(...), but the real implementation Logging::flush(...) instead.
	Logging::flush(nullptr);
	appenders.clear();
	layouts.clear();
}

void Logging::setUnblocked(bool isUnblocked) {
	logbook::setUnblocked(isUnblocked);
}

void Logging::setLevel(esl::logging::Level aLogLevel, const std::string& typeName) {
	logbook::Level logLevel = eslLoggingLevel2logbookLevel(aLogLevel);

	logbook::setLevel(logLevel, typeName);
}

bool Logging::isEnabled(const char* typeName, esl::logging::Level aLevel) {
	logbook::Level level = eslLoggingLevel2logbookLevel(aLevel);

	return logbook::isLoggingEnabled(typeName, level);
}

std::unique_ptr<esl::logging::OStream> Logging::createOStream(const esl::logging::Location& aLocation) {
	logbook::Level level = eslLoggingLevel2logbookLevel(aLocation.level);
	logbook::Location location(level, aLocation.object, aLocation.typeName, aLocation.function, aLocation.file, aLocation.line, aLocation.threadId);

	std::unique_ptr<logbook::Writer> writer = logbook::createWriter(location);

	return std::unique_ptr<esl::logging::OStream>(new OStream(*this, std::move(writer)));
}

unsigned int Logging::getThreadNo(std::thread::id threadId) {
	return logbook::getThreadNo(threadId);
}

void Logging::flush(std::ostream* oStream) {
	for(auto& appender : appenders) {
		if(oStream) {
			std::stringstream strStream;

			appender.second->flush(&strStream);
			if(!strStream.str().empty()) {
				(*oStream) << "\n\nFlush log messages from appender \"" << appender.first << "\":\n";
				(*oStream) << strStream.str();
			}
		}
		else {
			appender.second->flush(nullptr);
		}
	}
}

void Logging::addData(const std::string& configuration) {
	config::Logger loggerConfig(configuration);
	loggerConfig.install(*this);
}

void Logging::addFile(const boost::filesystem::path& filename) {
	config::Logger loggerConfig(filename);
	loggerConfig.install(*this);
}

void Logging::addLayout(const std::string& id, std::unique_ptr<esl::logging::Layout> layout) {
	if(layouts.insert(std::make_pair(id, std::move(layout))).second == false) {
		throw std::runtime_error("Cannot add layout with id \"" + id + "\" because it exists already");
	}
}

void Logging::addAppender(const std::string& name, const std::string& layoutRefId, std::unique_ptr<esl::logging::Appender> appender) {
	auto iter = layouts.find(layoutRefId);
	if(iter == std::end(layouts)) {
		throw std::runtime_error("Appender is referencing an undefined layout \"" + layoutRefId + "\"");
	}

	appender->setLayout(iter->second.get());

    appenders.push_back(std::make_pair(name, std::unique_ptr<Appender>(new Appender(std::move(appender)))));
}


} /* namespace logging */
} /* inline namespace v1_6 */
} /* namespace logbook4esl */
