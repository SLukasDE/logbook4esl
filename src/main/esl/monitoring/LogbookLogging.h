#ifndef ESL_MONITORING_LOGBOOKMONITORING_H_
#define ESL_MONITORING_LOGBOOKMONITORING_H_

#include <esl/monitoring/Appender.h>
#include <esl/monitoring/Layout.h>
#include <esl/monitoring/Logging.h>
#include <esl/monitoring/OStream.h>
#include <esl/monitoring/Streams.h>

#include <boost/filesystem/path.hpp>

#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace esl {
inline namespace v1_6 {
namespace monitoring {

class LogbookLogging : public Logging {
public:
	struct Settings {
		Settings() = default;
		Settings(const std::vector<std::pair<std::string, std::string>>& settings);
	};

	LogbookLogging();

	static std::unique_ptr<Logging> create(const std::vector<std::pair<std::string, std::string>>& settings);

	void setUnblocked(bool isUnblocked) override;

	void setLevel(Streams::Level logLevel, const std::string& typeName) override;

	bool isEnabled(const char* typeName, Streams::Level level) override;
	std::unique_ptr<OStream> createOStream(const Streams::Location& location) override;
	unsigned int getThreadNo(std::thread::id threadId) override;

	void flush(std::ostream* oStream) override;

	void addData(const std::string& configuration) override;
	void addFile(const boost::filesystem::path& filename) override;
	void addLayout(const std::string& id, std::unique_ptr<Layout> layout) override;
	void addAppender(const std::string& name, const std::string& layoutRefId, std::unique_ptr<Appender> appender) override;

private:
	LogbookLogging(const Settings& settings);

	std::unique_ptr<Logging> logging;
};

} /* namespace monitoring */
} /* inline namespace v1_6 */
} /* namespace esl */

#endif /* ESL_MONITORING_LOGBOOKMONITORING_H_ */
