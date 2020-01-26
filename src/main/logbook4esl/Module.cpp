/*
MIT License
Copyright (c) 2019, 2020 Sven Lukas

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
#include <esl/logging/Level.h>
#include <esl/module/Interface.h>

#include <memory>
#include <new>         // placement new
#include <type_traits> // aligned_storage

#include <vector>
#include <functional>
#include <mutex>

namespace logbook4esl {


namespace {
class Module : public esl::module::Module {
public:
	Module();

	std::recursive_mutex loggerMutex;
	std::vector<std::unique_ptr<Appender>> appenders;
};

typename std::aligned_storage<sizeof(Module), alignof(Module)>::type moduleBuffer; // memory for the object;
Module& module = reinterpret_cast<Module&> (moduleBuffer);
bool isInitialized = false;

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

bool& isEnabled(const char* typeName, esl::logging::Level aLevel) {
	logbook::Level level = eslLoggingLevel2logbookLevel(aLevel);

	return logbook::Internal::isEnabled(typeName, level);
}

std::pair<std::reference_wrapper<std::ostream>, void*> addWriter(const esl::logging::Location& aLocation, bool** enabled) {
	logbook::Level level = eslLoggingLevel2logbookLevel(aLocation.level);
	logbook::Location location(level, aLocation.object, aLocation.typeName, aLocation.function, aLocation.file, aLocation.line, aLocation.threadId);

	// check if pointer has been set already. So we call this expensive function only once.
	if((*enabled) == nullptr) {
		*enabled = &logbook::Internal::isEnabled(aLocation.typeName, level);
	}

	std::pair<logbook::Location*, std::reference_wrapper<std::ostream>> rv = logbook::Internal::pushCurrent(location, enabled);
	return std::make_pair(rv.second, static_cast<void*>(rv.first));
}

void removeWriter(std::ostream& ostream, void* data) {
	if(data) {
		logbook::Location& location(*static_cast<logbook::Location*>(data));
		logbook::Internal::popCurrent(static_cast<std::stringstream&>(ostream), location);
	}
	else {
		logbook::Internal::popCurrent();
	}
}

unsigned int getThreadNo(std::thread::id threadId) {
	return logbook::Internal::getThreadNo(threadId);
}

Module::Module()
: esl::module::Module()
{
	esl::module::Module::initialize(*this);

	addInterface(std::unique_ptr<const esl::module::Interface>(new esl::logging::Interface(
			getId(), "",
			setUnblocked, setLevel, addAppender, getAppenders, isEnabled, addWriter, removeWriter, getThreadNo)));
}

} /* anonymous namespace */

const esl::module::Module& getModule() {
	if(isInitialized == false) {
		/* ***************** *
		 * initialize module *
		 * ***************** */

		isInitialized = true;
		new (&module) Module(); // placement new
	}
	return module;
}

} /* namespace logbook4esl */
