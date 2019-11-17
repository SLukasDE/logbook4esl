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

#include <logbook4esl/Appender.h>
#include <esl/logging/Id.h>

namespace logbook4esl {
namespace {
esl::logging::Level logbookLevel2eslLoggingLevel(logbook::Level logLevel) {
	switch(logLevel) {
	case logbook::Level::TRACE:
		return esl::logging::Level::TRACE;
	case logbook::Level::DEBUG:
		return esl::logging::Level::DEBUG;
	case logbook::Level::INFO:
		return esl::logging::Level::INFO;
	case logbook::Level::WARN:
		return esl::logging::Level::WARN;
	case logbook::Level::ERROR:
		return esl::logging::Level::ERROR;
	case logbook::Level::SILENT:
		return esl::logging::Level::SILENT;
	default:
		break;
	}
	throw std::runtime_error("conversion error from logbook::Level to esl::logging::Level");
}

esl::logging::Id logbookId4eslLoggingId(const logbook::Id& id) {
	esl::logging::Id eslId(logbookLevel2eslLoggingLevel(id.level), id.object, id.typeName, id.function, id.file, id.line, id.threadId);
	return eslId;
}
}

Appender::Appender(esl::logging::Appender& aEslAppender)
: logbook::Appender(),
  eslAppender(aEslAppender)
{ }

void Appender::flushNewLine(const logbook::Id& id, bool enabled) {
	esl::logging::Interface::appenderFlushNewLine(eslAppender, logbookId4eslLoggingId(id), enabled);
}

void Appender::write(const logbook::Id& id, bool enabled, const char* ptr, std::size_t size) {
	esl::logging::Interface::appenderWrite(eslAppender, logbookId4eslLoggingId(id), enabled, ptr, size);
}

} /* namespace logbook4esl */
