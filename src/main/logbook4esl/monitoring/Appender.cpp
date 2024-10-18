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

#include <logbook4esl/monitoring/Appender.h>

#include <esl/monitoring/Streams.h>
#include <esl/monitoring/Logging.h>

namespace logbook4esl {
inline namespace v1_6 {
namespace monitoring {

namespace {
esl::monitoring::Streams::Level logbookLevel2eslLoggingLevel(logbook::Level logLevel) {
	switch(logLevel) {
	case logbook::Level::trace:
		return esl::monitoring::Streams::Level::trace;
	case logbook::Level::debug:
		return esl::monitoring::Streams::Level::debug;
	case logbook::Level::info:
		return esl::monitoring::Streams::Level::info;
	case logbook::Level::warn:
		return esl::monitoring::Streams::Level::warn;
	case logbook::Level::error:
		return esl::monitoring::Streams::Level::error;
	case logbook::Level::silent:
		return esl::monitoring::Streams::Level::silent;
	default:
		break;
	}
	throw std::runtime_error("conversion error from logbook::Level to esl::monitoring::Level");
}

esl::monitoring::Streams::Location logbookLocation2eslLoggingLocation(const logbook::Location& location) {
	esl::monitoring::Streams::Location eslLocation(logbookLevel2eslLoggingLevel(location.level), location.object, location.typeName, location.function, location.file, location.line, location.threadId);
	eslLocation.enabled = location.enabled;
	return eslLocation;
}
}

Appender::Appender(std::unique_ptr<esl::monitoring::Appender> aEslAppender)
: logbook::Appender(),
  eslAppender(std::move(aEslAppender))
{ }

void Appender::flush(std::ostream* oStream) {
	if(eslAppender) {
		eslAppender->flush(oStream);
	}
}

void Appender::flush() {
	flush(nullptr);
}

void Appender::write(const logbook::Location& location, const char* ptr, std::size_t size) {
	if(eslAppender) {
		eslAppender->write(logbookLocation2eslLoggingLocation(location), ptr, size);
	}
}

} /* namespace monitoring */
} /* inline namespace v1_6 */
} /* namespace logbook4esl */
