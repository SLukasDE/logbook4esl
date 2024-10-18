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

#include <logbook4esl/monitoring/Logging.h>
#include <logbook4esl/config/Logger.h>

#include <logbook/Logbook.h>
#include <logbook/Level.h>

#include <esl/system/Stacktrace.h>

#include <sstream>
#include <stdexcept>

namespace logbook4esl {
inline namespace v1_6 {
namespace monitoring {

namespace {
logbook::Level eslLoggingLevel2logbookLevel(esl::monitoring::Streams::Level logLevel) {
	switch(logLevel) {
	case esl::monitoring::Streams::Level::trace:
		return logbook::Level::trace;
	case esl::monitoring::Streams::Level::debug:
		return logbook::Level::debug;
	case esl::monitoring::Streams::Level::info:
		return logbook::Level::info;
	case esl::monitoring::Streams::Level::warn:
		return logbook::Level::warn;
	case esl::monitoring::Streams::Level::error:
		return logbook::Level::error;
	case esl::monitoring::Streams::Level::silent:
		return logbook::Level::silent;
	default:
		break;
	}
	throw std::runtime_error("conversion error from esl::monitoring::Level to logbook::Level");
}

class OStream : public esl::monitoring::OStream {
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

Logging::Logging(const esl::monitoring::LogbookLogging::Settings&)
{ }

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

void Logging::setLevel(esl::monitoring::Streams::Level aLogLevel, const std::string& typeName) {
	logbook::Level logLevel = eslLoggingLevel2logbookLevel(aLogLevel);

	logbook::setLevel(logLevel, typeName);
}

bool Logging::isEnabled(const char* typeName, esl::monitoring::Streams::Level aLevel) {
	logbook::Level level = eslLoggingLevel2logbookLevel(aLevel);

	return logbook::isLoggingEnabled(typeName, level);
}

std::unique_ptr<esl::monitoring::OStream> Logging::createOStream(const esl::monitoring::Streams::Location& aLocation) {
	logbook::Level level = eslLoggingLevel2logbookLevel(aLocation.level);
	logbook::Location location(level, aLocation.object, aLocation.typeName, aLocation.function, aLocation.file, aLocation.line, aLocation.threadId);

	std::unique_ptr<logbook::Writer> writer = logbook::createWriter(location);

	return std::unique_ptr<esl::monitoring::OStream>(new OStream(*this, std::move(writer)));
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
	config::Logger loggerConfig(false, configuration);
	loggerConfig.install(*this);
}

void Logging::addFile(const std::string& filename) {
	config::Logger loggerConfig(true, filename);
	loggerConfig.install(*this);
}

void Logging::addLayout(const std::string& id, std::unique_ptr<esl::monitoring::Layout> layout) {
	if(layouts.insert(std::make_pair(id, std::move(layout))).second == false) {
		throw std::runtime_error("Cannot add layout with id \"" + id + "\" because it exists already");
	}
}

void Logging::addAppender(const std::string& name, const std::string& layoutRefId, std::unique_ptr<esl::monitoring::Appender> appender) {
	auto iter = layouts.find(layoutRefId);
	if(iter == std::end(layouts)) {
		throw std::runtime_error("Appender is referencing an undefined layout \"" + layoutRefId + "\"");
	}

	appender->setLayout(iter->second.get());

    appenders.push_back(std::make_pair(name, std::unique_ptr<Appender>(new Appender(std::move(appender)))));
}


} /* namespace monitoring */
} /* inline namespace v1_6 */
} /* namespace logbook4esl */
