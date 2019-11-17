/*
MIT License
Copyright (c) 2019 Sven Lukas

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
#include <logbook/Logger.h>
#include <logbook/Internal.h>

#include <esl/logging/Interface.h>
#include <esl/logging/Appender.h>
#include <esl/bootstrap/Interface.h>
#include <new>         // placement new
#include <type_traits> // aligned_storage

#include <vector>
#include <functional>
#include <mutex>

namespace logbook4esl {


namespace {
class Module : public esl::bootstrap::Module {
public:
	Module() = default;
	~Module() = default;

	static void initialize();

	std::recursive_mutex loggerMutex;
	std::vector<std::unique_ptr<Appender>> appenders;

private:
	esl::logging::Interface interfaceLogging;
};

typename std::aligned_storage<sizeof(Module), alignof(Module)>::type moduleBuffer; // memory for the object;
Module& module = reinterpret_cast<Module&> (moduleBuffer);

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

void setUnblocked(bool isUnblocked) {
	logbook::Logger::setUnblocked(isUnblocked);
}

void setLevel(esl::logging::Level aLogLevel, const std::string& typeName) {
	logbook::Level logLevel = eslLoggingLevel2logbookLevel(aLogLevel);
	logbook::Logger::setLevel(logLevel, typeName);
}


void addAppender(esl::logging::Appender& appender) {
    std::lock_guard<std::recursive_mutex> loggerLock(module.loggerMutex);

    Appender* appenderPtr = new Appender(appender);
	module.appenders.push_back(std::unique_ptr<Appender>(appenderPtr));

    logbook::Logger::addAppender(*appenderPtr);
}

std::vector<std::reference_wrapper<esl::logging::Appender>> getAppenders() {
	std::vector<std::reference_wrapper<esl::logging::Appender>> rv;

	{
	    std::lock_guard<std::recursive_mutex> loggerLock(module.loggerMutex);
		for(auto& appender : module.appenders) {
			rv.push_back(std::ref(appender->eslAppender));
		}
	}

	return rv;
}


std::pair<std::reference_wrapper<std::ostream>, void*> addWriter(esl::logging::Id aId, bool** enabled) {
	logbook::Level level = eslLoggingLevel2logbookLevel(aId.level);
	logbook::Id id(level, aId.object, aId.typeName, aId.function, aId.file, aId.line, aId.threadId);
	std::pair<logbook::Id*, std::reference_wrapper<std::ostream>> rv = logbook::Internal::pushCurrent(id, enabled);
	return std::make_pair(rv.second, static_cast<void*>(rv.first));
}

void removeWriter(std::ostream& ostream, void* data) {
	if(data) {
		logbook::Id& id(*static_cast<logbook::Id*>(data));
		logbook::Internal::popCurrent(static_cast<std::stringstream&>(ostream), id);
	}
	else {
		logbook::Internal::popCurrent();
	}
}

unsigned int getThreadNo(std::thread::id threadId) {
	return logbook::Internal::getThreadNo(threadId);
}

void Module::initialize() {
	static bool isInitialized = false;

	if(isInitialized == false) {
		isInitialized = true;

		/* ***************** *
		 * initialize module *
		 * ***************** */
		new (&module) Module(); // placement new
		esl::bootstrap::Module::initialize(module);
		esl::logging::Interface::initialize(module.interfaceLogging, setUnblocked, setLevel, addAppender, getAppenders, addWriter, removeWriter, getThreadNo);
		module.interfacesProvided.next = &module.interfaceLogging;
	}
}
}

const esl::bootstrap::Module& getModule() {
	Module::initialize();
	return module;
}

} /* namespace logbook4esl */
