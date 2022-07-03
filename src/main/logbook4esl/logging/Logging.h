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

#include <esl/logging/Logging.h>
#include <esl/logging/Appender.h>
#include <esl/logging/Level.h>
#include <esl/logging/Location.h>
#include <esl/logging/OStream.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace logbook4esl {
namespace logging {

class Logging : public esl::logging::Logging {
public:
	static std::unique_ptr<esl::logging::Logging> create(const std::vector<std::pair<std::string, std::string>>& settings);

	Logging(const std::vector<std::pair<std::string, std::string>>& settings);

	void setUnblocked(bool isUnblocked) override;
	void setLevel(esl::logging::Level logLevel, const std::string& typeName) override;
	void* addAppender(esl::logging::Appender& appender) override;
	void removeAppender(void* handle) override;
	bool isEnabled(const char* typeName, esl::logging::Level level) override;
	std::unique_ptr<esl::logging::OStream> createOStream(const esl::logging::Location& location) override;
	unsigned int getThreadNo(std::thread::id threadId) override;
};

} /* namespace logging */
} /* namespace logbook4esl */

#endif /* LOGBOOK4ESL_LOGGING_LOGGING_H_ */
