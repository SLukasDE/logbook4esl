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

#ifndef LOGBOOK4ESL_MONITORING_MONITORING_H_
#define LOGBOOK4ESL_MONITORING_MONITORING_H_

#include <logbook4esl/monitoring/Appender.h>

#include <esl/monitoring/Logging.h>
#include <esl/monitoring/LogbookLogging.h>

#include <esl/monitoring/Appender.h>
#include <esl/monitoring/Layout.h>
#include <esl/monitoring/OStream.h>
#include <esl/monitoring/Streams.h>

#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace logbook4esl {
inline namespace v1_6 {
namespace monitoring {

class Logging : public esl::monitoring::Logging {
public:
	Logging(const esl::monitoring::LogbookLogging::Settings& settings);
	~Logging();

	// NOT thread save - call it at the beginning if needed. Default is already "true"
	// unblocked behavior makes other threads not waiting on logging, while current thread is writing to logger already.
	// If logger is used already by current thread, other threads will write to a temporary buffer.
	// - Temporary buffer is flushed to real logger, if other thread is done using the logger.
	// - If logger is still used by current thread, buffer is queued.
	// - If current thread is done using the logger, it flushes queued buffers.
	void setUnblocked(bool isUnblocked) override;

	// thread safe, quaranteed by configMutex
	void setLevel(esl::monitoring::Streams::Level logLevel, const std::string& typeName) override;

	bool isEnabled(const char* typeName, esl::monitoring::Streams::Level level) override;
	std::unique_ptr<esl::monitoring::OStream> createOStream(const esl::monitoring::Streams::Location& location) override;
	unsigned int getThreadNo(std::thread::id threadId) override;

	void flush(std::ostream* oStream) override;

	void addData(const std::string& configuration) override;
	void addFile(const std::string& filename) override;

	void addLayout(const std::string& id, std::unique_ptr<esl::monitoring::Layout> layout) override;

	// thread safe, quaranteed by loggerMutex
	void addAppender(const std::string& name, const std::string& layoutRefId, std::unique_ptr<esl::monitoring::Appender> appender) override;

private:
	std::map<std::string, std::unique_ptr<esl::monitoring::Layout>> layouts;
	std::vector<std::pair<std::string, std::unique_ptr<Appender>>> appenders;
};

} /* namespace monitoring */
} /* inline namespace v1_6 */
} /* namespace logbook4esl */

#endif /* LOGBOOK4ESL_MONITORING_MONITORING_H_ */
