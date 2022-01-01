/*
MIT License
Copyright (c) 2019-2022 Sven Lukas

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

#include <logbook4esl/Module.h>
#include <logbook4esl/Appender.h>
#include <logbook/Logbook.h>
#include <logbook/Level.h>

#include <esl/logging/Interface.h>
#include <esl/logging/Appender.h>
#include <esl/logging/Level.h>
#include <esl/logging/Location.h>
#include <esl/logging/OStream.h>
#include <esl/Module.h>

#include <memory>

namespace logbook4esl {
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

void setLevel(esl::logging::Level aLogLevel, const std::string& typeName) {
	logbook::Level logLevel = eslLoggingLevel2logbookLevel(aLogLevel);

	logbook::setLevel(logLevel, typeName);
}

void* addAppender(esl::logging::Appender& appender) {
	return new Appender(appender);
}

void removeAppender(void* handle) {
	if(handle) {
		Appender* appender = static_cast<Appender*>(handle);
		delete appender;
	}
}

bool isEnabled(const char* typeName, esl::logging::Level aLevel) {
	logbook::Level level = eslLoggingLevel2logbookLevel(aLevel);

	return logbook::isLoggingEnabled(typeName, level);
}

class OStream : public esl::logging::OStream {
public:
	OStream(std::unique_ptr<logbook::Writer> aWriter)
	: writer(std::move(aWriter))
	{ }

    std::ostream* getOStream() override {
    	if(writer) {
        	return writer->getOStream();
    	}
    	return nullptr;
    }

private:
    std::unique_ptr<logbook::Writer> writer;
};

std::unique_ptr<esl::logging::OStream> createWriter(const esl::logging::Location& aLocation) {
	logbook::Level level = eslLoggingLevel2logbookLevel(aLocation.level);
	logbook::Location location(level, aLocation.object, aLocation.typeName, aLocation.function, aLocation.file, aLocation.line, aLocation.threadId);

	std::unique_ptr<logbook::Writer> writer = logbook::createWriter(location);

	return std::unique_ptr<esl::logging::OStream>(new OStream(std::move(writer)));
}

unsigned int getThreadNo(std::thread::id threadId) {
	return logbook::getThreadNo(threadId);
}
} /* anonymous namespace */

void Module::install(esl::module::Module& module) {
	esl::setModule(module);

	module.addInterface(esl::logging::Interface::createInterface(
			"logbook4esl",
			logbook::setUnblocked, setLevel, addAppender, removeAppender, isEnabled, createWriter, getThreadNo));
}

} /* namespace logbook4esl */
