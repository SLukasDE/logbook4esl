#ifndef LOGBOOK4ESL_CONFIG_APPENDER_H_
#define LOGBOOK4ESL_CONFIG_APPENDER_H_

#include <common4esl/config/Config.h>
#include <common4esl/config/Setting.h>

#include <esl/monitoring/Appender.h>

#include <tinyxml2/tinyxml2.h>

#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace logbook4esl {
inline namespace v1_6 {
namespace config {

class Appender : public common4esl::config::Config {
public:
	Appender(const std::string& fileName, const tinyxml2::XMLElement& element);

	const std::string& getName() const;
	const std::string& getLayoutId() const;

	void save(std::ostream& oStream, std::size_t spaces) const;

	std::unique_ptr<esl::monitoring::Appender> create() const;

private:
	std::string name;
	esl::monitoring::Appender::RecordLevel recordLevel = esl::monitoring::Appender::RecordLevel::SELECTED;
	std::string layoutId;
	std::string implementation;
	std::vector<common4esl::config::Setting> parameters;

	void parseInnerElement(const tinyxml2::XMLElement& element);
};

} /* namespace config */
} /* inline namespace v1_6 */
} /* namespace logbook4esl */

#endif /* LOGBOOK4ESL_CONFIG_APPENDER_H_ */
