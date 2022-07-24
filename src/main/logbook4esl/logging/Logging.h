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

#ifndef LOGBOOK4ESL_LOGGING_LOGGING_H_
#define LOGBOOK4ESL_LOGGING_LOGGING_H_

#include <logbook4esl/logging/Appender.h>

#include <esl/logging/Appender.h>
#include <esl/logging/Layout.h>
#include <esl/logging/Level.h>
#include <esl/logging/Location.h>
#include <esl/logging/Logging.h>
#include <esl/logging/OStream.h>
#include <esl/logging/StreamReal.h>

#include <boost/filesystem/path.hpp>

#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace logbook4esl {
namespace logging {

class Logging : public esl::logging::Logging {
public:
	static std::unique_ptr<esl::logging::Logging> create(const std::vector<std::pair<std::string, std::string>>& settings);

	Logging(const std::vector<std::pair<std::string, std::string>>& settings);

	// NOT thread save - call it at the beginning if needed. Default is already "true"
	// unblocked behavior makes other threads not waiting on logging, while current thread is writing to logger already.
	// If logger is used already by current thread, other threads will write to a temporary buffer.
	// - Temporary buffer is flushed to real logger, if other thread is done using the logger.
	// - If logger is still used by current thread, buffer is queued.
	// - If current thread is done using the logger, it flushes queued buffers.
	void setUnblocked(bool isUnblocked) override;

	// thread safe, quaranteed by configMutex
	void setLevel(esl::logging::Level logLevel, const std::string& typeName) override;

	bool isEnabled(const char* typeName, esl::logging::Level level) override;
	std::unique_ptr<esl::logging::OStream> createOStream(const esl::logging::Location& location) override;
	unsigned int getThreadNo(std::thread::id threadId) override;

	void flush(std::ostream* oStream) override;

	void addData(const std::string& configuration) override;
	void addFile(const boost::filesystem::path& filename) override;

	void addLayout(const std::string& id, std::unique_ptr<esl::logging::Layout> layout) override;

	// thread safe, quaranteed by loggerMutex
	void addAppender(const std::string& name, const std::string& layoutRefId, std::unique_ptr<esl::logging::Appender> appender) override;

private:
	std::map<std::string, std::unique_ptr<esl::logging::Layout>> layouts;
	std::vector<std::pair<std::string, std::unique_ptr<Appender>>> appenders;
	//std::vector<std::pair<std::string, std::unique_ptr<esl::logging::Appender>>> appenders;
};

} /* namespace logging */
} /* namespace logbook4esl */

#endif /* LOGBOOK4ESL_LOGGING_LOGGING_H_ */
